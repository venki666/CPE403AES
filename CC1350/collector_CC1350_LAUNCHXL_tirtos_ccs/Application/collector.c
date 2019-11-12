/******************************************************************************

 @file collector.c

 @brief TIMAC 2.0 Collector Example Application

 Group: WCS LPC
 Target Device: cc13x0

 ******************************************************************************
 
 Copyright (c) 2016-2019, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/
#include <string.h>
#include <stdint.h>

#include "mac_util.h"
#include "api_mac.h"
#include "cllc.h"
#include "csf.h"
#include "smsgs.h"
#include "collector.h"
#include "board_lcd.h"
#ifdef FEATURE_SECURE_COMMISSIONING
#include "sm_ti154.h"
#endif

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

#if !defined(STATIC)
/* make local */
#define STATIC static
#endif

#if !defined(CONFIG_AUTO_START)
#if defined(AUTO_START)
#define CONFIG_AUTO_START 1
#else
#define CONFIG_AUTO_START 0
#endif
#endif

/* Beacon order for non beacon network */
#define NON_BEACON_ORDER      15

/* Default MSDU Handle rollover */
#define MSDU_HANDLE_MAX 0x1F

/* App marker in MSDU handle */
#define APP_MARKER_MSDU_HANDLE 0x80

/* App Config request marker for the MSDU handle */
#define APP_CONFIG_MSDU_HANDLE 0x40

/* Ramp data request marker for the MSDU handle */
#define RAMP_DATA_MSDU_HANDLE 0x20
/* App Broadcast Cmd Msg marker for the MSDU Handle */
#define APP_BROADCAST_MSDU_HANDLE 0x20

/* Default configuration frame control */
#define CONFIG_FRAME_CONTROL (Smsgs_dataFields_tempSensor | \
                              Smsgs_dataFields_lightSensor | \
                              Smsgs_dataFields_humiditySensor | \
                              Smsgs_dataFields_msgStats | \
                              Smsgs_dataFields_configSettings)

/* Delay for config request retry in busy network */
#define CONFIG_DELAY 2000
#define CONFIG_RESPONSE_DELAY 3*CONFIG_DELAY
/* Tracking timeouts */
#define TRACKING_CNF_DELAY_TIME 2000 /* in milliseconds */
#if (CONFIG_PHY_ID == APIMAC_PHY_ID_NONE) && (CONFIG_MAC_BEACON_ORDER != NON_BEACON_ORDER)
#define TRACKING_TIMEOUT_TIME ((1<<CONFIG_MAC_BEACON_ORDER) * 960 * 16 * 3 / 1000) /*in milliseconds*/
#else
#define TRACKING_TIMEOUT_TIME (CONFIG_POLLING_INTERVAL * 3) /*in milliseconds*/
#endif
/* Initial delay before broadcast transmissions are started in FH mode */
#define BROADCAST_CMD_START_TIME 60000

/* Number of superframe periods to hold a indirect packet at collector for
Sensor to poll and get the frame*/
#define BCN_MODE_INDIRECT_PERSISTENT_TIME 3

#if ((CONFIG_PHY_ID >= APIMAC_MRFSK_STD_PHY_ID_BEGIN) && (CONFIG_PHY_ID <= APIMAC_MRFSK_GENERIC_PHY_ID_BEGIN))

/* MAC Indirect Persistent Timeout */
#define INDIRECT_PERSISTENT_TIME ((5 * 1000 * CONFIG_POLLING_INTERVAL / 2)/ \
                                  (BASE_SUPER_FRAME_DURATION * \
                                   SYMBOL_DURATION_50_kbps))

#elif ((CONFIG_PHY_ID >= APIMAC_GENERIC_US_915_PHY_132) && (CONFIG_PHY_ID <= APIMAC_GENERIC_ETSI_863_PHY_133))

/* MAC Indirect Persistent Timeout */
#define INDIRECT_PERSISTENT_TIME ((5 * 1000 * CONFIG_POLLING_INTERVAL / 2)/ \
                                  (BASE_SUPER_FRAME_DURATION * \
                                   SYMBOL_DURATION_200_kbps))

#elif (CONFIG_PHY_ID == APIMAC_PHY_ID_NONE)

/* MAC Indirect Persistent Timeout */
#define INDIRECT_PERSISTENT_TIME ((5 * 1000 * CONFIG_POLLING_INTERVAL / 2)/ \
                                  (BASE_SUPER_FRAME_DURATION * \
                                   SYMBOL_DURATION_250_kbps))

#else
/* MAC Indirect Persistent Timeout */
#define INDIRECT_PERSISTENT_TIME ((5 * 1000 * CONFIG_POLLING_INTERVAL / 2)/ \
                                  (BASE_SUPER_FRAME_DURATION * \
                                    SYMBOL_DURATION_LRM))
#endif

/* Assoc Table (CLLC) status settings */
#define ASSOC_CONFIG_SENT       0x0100    /* Config Req sent */
#define ASSOC_CONFIG_RSP        0x0200    /* Config Rsp received */
#define ASSOC_CONFIG_MASK       0x0300    /* Config mask */
#define ASSOC_TRACKING_SENT     0x1000    /* Tracking Req sent */
#define ASSOC_TRACKING_RSP      0x2000    /* Tracking Rsp received */
#define ASSOC_TRACKING_RETRY    0x4000    /* Tracking Req retried */
#define ASSOC_TRACKING_ERROR    0x8000    /* Tracking Req error */
#define ASSOC_TRACKING_MASK     0xF000    /* Tracking mask  */
/******************************************************************************
 Global variables
 *****************************************************************************/

/* Task pending events */
uint16_t Collector_events = 0;

/*! Collector statistics */
Collector_statistics_t Collector_statistics;

/* Permit join setting */
extern bool permitJoining;
/******************************************************************************
 Local variables
 *****************************************************************************/

static void *sem;

/*! true if the device was restarted */
static bool restarted = false;

/*! CLLC State */
STATIC Cllc_states_t cllcState = Cllc_states_initWaiting;

/*! Device's PAN ID */
STATIC uint16_t devicePanId = 0xFFFF;

/*! Device's Outgoing MSDU Handle values */
STATIC uint8_t deviceTxMsduHandle = 0;

STATIC bool fhEnabled = false;

/******************************************************************************
 Local function prototypes
 *****************************************************************************/
static void initializeClocks(void);
static void cllcStartedCB(Llc_netInfo_t *pStartedInfo);
static ApiMac_assocStatus_t cllcDeviceJoiningCB(
                ApiMac_deviceDescriptor_t *pDevInfo,
                ApiMac_capabilityInfo_t *pCapInfo);
static void cllcStateChangedCB(Cllc_states_t state);
static void dataCnfCB(ApiMac_mcpsDataCnf_t *pDataCnf);
static void dataIndCB(ApiMac_mcpsDataInd_t *pDataInd);
static void processStartEvent(void);
static void processConfigResponse(ApiMac_mcpsDataInd_t *pDataInd);
static void processTrackingResponse(ApiMac_mcpsDataInd_t *pDataInd);
static void processToggleLedResponse(ApiMac_mcpsDataInd_t *pDataInd);
static void processSensorData(ApiMac_mcpsDataInd_t *pDataInd);
static Cllc_associated_devices_t *findDevice(ApiMac_sAddr_t *pAddr);
static Cllc_associated_devices_t *findDeviceStatusBit(uint16_t mask, uint16_t statusBit);
static uint8_t getMsduHandle(Smsgs_cmdIds_t msgType);
static bool sendMsg(Smsgs_cmdIds_t type, uint16_t dstShortAddr, bool rxOnIdle,
                    uint16_t len,
                    uint8_t *pData);
static void generateConfigRequests(void);
static void generateTrackingRequests(void);
static void generateBroadcastCmd(void);
static void sendTrackingRequest(Cllc_associated_devices_t *pDev);
static void commStatusIndCB(ApiMac_mlmeCommStatusInd_t *pCommStatusInd);
static void pollIndCB(ApiMac_mlmePollInd_t *pPollInd);
static void processDataRetry(ApiMac_sAddr_t *pAddr);
static void processConfigRetry(void);
static void processIdendifyLedRequest(ApiMac_mcpsDataInd_t *pDataInd);
static void orphanIndCb(ApiMac_mlmeOrphanInd_t *pData);
#ifdef FEATURE_SECURE_COMMISSIONING
/* Security Manager callback functions */
static void smFailCMProcessCb(ApiMac_deviceDescriptor_t *devInfo, bool rxOnIdle, bool keyRefreshment);
static void smSuccessCMProcessCb(ApiMac_deviceDescriptor_t *devInfo, bool keyRefreshment);
#endif /* FEATURE_SECURE_COMMISSIONING */

#ifdef POWER_MEAS
void generateIndirectRampMsg(void);
#endif

/******************************************************************************
 Callback tables
 *****************************************************************************/

/*! API MAC Callback table */
ApiMac_callbacks_t Collector_macCallbacks =
    {
      /*! Associate Indicated callback */
      NULL,
      /*! Associate Confirmation callback */
      NULL,
      /*! Disassociate Indication callback */
      NULL,
      /*! Disassociate Confirmation callback */
      NULL,
      /*! Beacon Notify Indication callback */
      NULL,
      /*! Orphan Indication callback */
      orphanIndCb,
      /*! Scan Confirmation callback */
      NULL,
      /*! Start Confirmation callback */
      NULL,
      /*! Sync Loss Indication callback */
      NULL,
      /*! Poll Confirm callback */
      NULL,
      /*! Comm Status Indication callback */
      commStatusIndCB,
      /*! Poll Indication Callback */
      pollIndCB,
      /*! Data Confirmation callback */
      dataCnfCB,
      /*! Data Indication callback */
      dataIndCB,
      /*! Purge Confirm callback */
      NULL,
      /*! WiSUN Async Indication callback */
      NULL,
      /*! WiSUN Async Confirmation callback */
      NULL,
      /*! Unprocessed message callback */
      NULL
    };

STATIC Cllc_callbacks_t cllcCallbacks =
    {
      /*! Coordinator Started Indication callback */
      cllcStartedCB,
      /*! Device joining callback */
      cllcDeviceJoiningCB,
      /*! The state has changed callback */
      cllcStateChangedCB
    };
#ifdef FEATURE_SECURE_COMMISSIONING
STATIC SM_callbacks_t SMCallbacks =
    {
      /*! Security authentication failed callback */
      smFailCMProcessCb,
      /* Security authentication successful callback */
      smSuccessCMProcessCb
    };
#endif /* FEATURE_SECURE_COMMISSIONING */

/******************************************************************************
 Public Functions
 *****************************************************************************/

/*!
 Initialize this application.

 Public function defined in collector.h
 */
#ifdef OSAL_PORT2TIRTOS
void Collector_init(uint8_t macTaskId)
#else
void Collector_init(void)
#endif
{
    /* Initialize the collector's statistics */
    memset(&Collector_statistics, 0, sizeof(Collector_statistics_t));

    /* Initialize the MAC */
#ifdef OSAL_PORT2TIRTOS
    sem = ApiMac_init(macTaskId,CONFIG_FH_ENABLE);
#else
    sem = ApiMac_init(CONFIG_FH_ENABLE);
#endif

    ApiMac_mlmeSetReqUint8(ApiMac_attribute_phyCurrentDescriptorId,
                           (uint8_t)CONFIG_PHY_ID);

    ApiMac_mlmeSetReqUint8(ApiMac_attribute_channelPage,
                           (uint8_t)CONFIG_CHANNEL_PAGE);

    /* Initialize the Coordinator Logical Link Controller */
    Cllc_init(&Collector_macCallbacks, &cllcCallbacks);

    /* Register the MAC Callbacks */
    ApiMac_registerCallbacks(&Collector_macCallbacks);

    /* Initialize the platform specific functions */
    Csf_init(sem);

#ifdef FEATURE_SECURE_COMMISSIONING
    /* Intialize the security manager and register callbacks */
    SM_init();
    SM_registerCallback(&SMCallbacks);
#endif /* FEATURE_SECURE_COMMISSIONING */
    /* Set the indirect persistent timeout */
#if (CONFIG_MAC_BEACON_ORDER != NON_BEACON_ORDER)
    ApiMac_mlmeSetReqUint16(ApiMac_attribute_transactionPersistenceTime,
                            BCN_MODE_INDIRECT_PERSISTENT_TIME);
#else
    ApiMac_mlmeSetReqUint16(ApiMac_attribute_transactionPersistenceTime,
                            INDIRECT_PERSISTENT_TIME);
#endif
    ApiMac_mlmeSetReqUint8(ApiMac_attribute_phyTransmitPowerSigned,
                           (uint8_t)CONFIG_TRANSMIT_POWER);
    /* Set Min BE */
    ApiMac_mlmeSetReqUint8(ApiMac_attribute_backoffExponent,
                              (uint8_t)CONFIG_MIN_BE);
    /* Set Max BE */
    ApiMac_mlmeSetReqUint8(ApiMac_attribute_maxBackoffExponent,
                              (uint8_t)CONFIG_MAX_BE);
    /* Set MAC MAX CSMA Backoffs */
    ApiMac_mlmeSetReqUint8(ApiMac_attribute_maxCsmaBackoffs,
                              (uint8_t)CONFIG_MAC_MAX_CSMA_BACKOFFS);
    /* Set MAC MAX Frame Retries */
    ApiMac_mlmeSetReqUint8(ApiMac_attribute_maxFrameRetries,
                              (uint8_t)CONFIG_MAX_RETRIES);
#ifdef FCS_TYPE16
    /* Set the fcs type */
    ApiMac_mlmeSetReqBool(ApiMac_attribute_fcsType,
                           (bool)1);
#endif

    /* Initialize the app clocks */
    initializeClocks();
    if(CONFIG_FH_ENABLE && (FH_BROADCAST_DWELL_TIME > 0))
    {
        /* Start broadcast frame transmissions in FH mode if broadcast dwell time
         * is greater than zero */
        Csf_setBroadcastClock(BROADCAST_CMD_START_TIME);
    }

    if(CONFIG_AUTO_START)
    {
        /* Start the device */
        Util_setEvent(&Collector_events, COLLECTOR_START_EVT);
    }
}

/*!
 Application task processing.

 Public function defined in collector.h
 */
void Collector_process(void)
{
    /* Start the collector device in the network */
    if(Collector_events & COLLECTOR_START_EVT)
    {
        if(cllcState == Cllc_states_initWaiting)
        {
            processStartEvent();
        }

        /* Clear the event */
        Util_clearEvent(&Collector_events, COLLECTOR_START_EVT);
    }

    /* Is it time to send the next tracking message? */
    if(Collector_events & COLLECTOR_TRACKING_TIMEOUT_EVT)
    {
        /* Process Tracking Event */
        generateTrackingRequests();

        /* Clear the event */
        Util_clearEvent(&Collector_events, COLLECTOR_TRACKING_TIMEOUT_EVT);
    }

    /*
     The generate a config request for all associated devices that need one
     */
    if(Collector_events & COLLECTOR_CONFIG_EVT)
    {
#ifdef FEATURE_SECURE_COMMISSIONING
        if(SM_Last_State != SM_CM_InProgress) {
            generateConfigRequests();
        }
#else
        generateConfigRequests();
#endif
        /* Clear the event */
        Util_clearEvent(&Collector_events, COLLECTOR_CONFIG_EVT);
    }

    /*
     Collector generate a broadcast command message for FH mode
     */
    if(Collector_events & COLLECTOR_BROADCAST_TIMEOUT_EVT)
    {
        /* Clear the event */
        Util_clearEvent(&Collector_events, COLLECTOR_BROADCAST_TIMEOUT_EVT);
        if(FH_BROADCAST_INTERVAL > 0 && (!CERTIFICATION_TEST_MODE))
        {
            generateBroadcastCmd();
            /* set clock for next broadcast command */
            Csf_setBroadcastClock(FH_BROADCAST_INTERVAL);
        }
    }

    /* Process LLC Events */
    Cllc_process();

    /* Allow the Specific functions to process */
    Csf_processEvents();

#ifdef FEATURE_SECURE_COMMISSIONING
    /* Allow the security manager specific functions to process */
    SM_process();
#endif /* FEATURE_SECURE_COMMISSIONING */
    /*
     Don't process ApiMac messages until all of the collector events
     are processed.
     */
    if(Collector_events == 0)
    {
        /* Wait for response message or events */
        ApiMac_processIncoming();
    }
}

/*!
 Build and send the configuration message to a device.

 Public function defined in collector.h
 */
Collector_status_t Collector_sendConfigRequest(ApiMac_sAddr_t *pDstAddr,
                                               uint16_t frameControl,
                                               uint32_t reportingInterval,
                                               uint32_t pollingInterval)
{
    Collector_status_t status = Collector_status_invalid_state;

    /* Are we in the right state? */
    if(cllcState >= Cllc_states_started)
    {
        Llc_deviceListItem_t item;

        /* Is the device a known device? */
        if(Csf_getDevice(pDstAddr, &item))
        {
            uint8_t buffer[SMSGS_CONFIG_REQUEST_MSG_LENGTH];
            uint8_t *pBuf = buffer;

            /* Build the message */
            *pBuf++ = (uint8_t)Smsgs_cmdIds_configReq;
            *pBuf++ = Util_loUint16(frameControl);
            *pBuf++ = Util_hiUint16(frameControl);
            *pBuf++ = Util_breakUint32(reportingInterval, 0);
            *pBuf++ = Util_breakUint32(reportingInterval, 1);
            *pBuf++ = Util_breakUint32(reportingInterval, 2);
            *pBuf++ = Util_breakUint32(reportingInterval, 3);
            *pBuf++ = Util_breakUint32(pollingInterval, 0);
            *pBuf++ = Util_breakUint32(pollingInterval, 1);
            *pBuf++ = Util_breakUint32(pollingInterval, 2);
            *pBuf = Util_breakUint32(pollingInterval, 3);

            if((sendMsg(Smsgs_cmdIds_configReq, item.devInfo.shortAddress,
                        item.capInfo.rxOnWhenIdle,
                        (SMSGS_CONFIG_REQUEST_MSG_LENGTH),
                         buffer)) == true)
            {
                status = Collector_status_success;
                Collector_statistics.configRequestAttempts++;
                /* set timer for retry in case response is not received */
                Csf_setConfigClock(CONFIG_DELAY);
            }
            else
            {
                processConfigRetry();
            }
        }
    }

    return (status);
}

/*!
 Update the collector statistics

 Public function defined in collector.h
 */
void Collector_updateStats( void )
{
    /* update the stats from the MAC */
    ApiMac_mlmeGetReqUint32(ApiMac_attribute_diagRxSecureFail,
                            &Collector_statistics.rxDecryptFailures);

    ApiMac_mlmeGetReqUint32(ApiMac_attribute_diagTxSecureFail,
                            &Collector_statistics.txEncryptFailures);
}

/*!
 Build and send the toggle led message to a device.

 Public function defined in collector.h
 */
Collector_status_t Collector_sendToggleLedRequest(ApiMac_sAddr_t *pDstAddr)
{
    Collector_status_t status = Collector_status_invalid_state;

    /* Are we in the right state? */
    if(cllcState >= Cllc_states_started)
    {
        Llc_deviceListItem_t item;

        /* Is the device a known device? */
        if(Csf_getDevice(pDstAddr, &item))
        {
            uint8_t buffer[SMSGS_TOGGLE_LED_REQUEST_MSG_LEN];

            /* Build the message */
            buffer[0] = (uint8_t)Smsgs_cmdIds_toggleLedReq;

            sendMsg(Smsgs_cmdIds_toggleLedReq, item.devInfo.shortAddress,
                    item.capInfo.rxOnWhenIdle,
                    SMSGS_TOGGLE_LED_REQUEST_MSG_LEN,
                    buffer);

            status = Collector_status_success;
        }
        else
        {
            status = Collector_status_deviceNotFound;
        }
    }

    return(status);
}

/******************************************************************************
 Local Functions
 *****************************************************************************/

/*!
 * @brief       Initialize the clocks.
 */
static void initializeClocks(void)
{
    /* Initialize the tracking clock */
    Csf_initializeTrackingClock();
    Csf_initializeConfigClock();
    Csf_initializeBroadcastClock();
    Csf_initializeIdentifyClock();
}

/*!
 * @brief      CLLC Started callback.
 *
 * @param      pStartedInfo - pointer to network information
 */
static void cllcStartedCB(Llc_netInfo_t *pStartedInfo)
{
    devicePanId = pStartedInfo->devInfo.panID;
    if(pStartedInfo->fh == true)
    {
        fhEnabled = true;
    }

    /* updated the user */
    Csf_networkUpdate(restarted, pStartedInfo);

    /* Start the tracking clock */
    Csf_setTrackingClock(TRACKING_DELAY_TIME);
#ifdef FEATURE_SECURE_COMMISSIONING
    /* Coordinator has started */
    readySMToRun = true;
#endif
}

/*!
 * @brief      Device Joining callback from the CLLC module (ref.
 *             Cllc_deviceJoiningFp_t in cllc.h).  This function basically
 *             gives permission that the device can join with the return
 *             value.
 *
 * @param      pDevInfo - device information
 * @param      capInfo - device's capability information
 *
 * @return     ApiMac_assocStatus_t
 */
static ApiMac_assocStatus_t cllcDeviceJoiningCB(
                ApiMac_deviceDescriptor_t *pDevInfo,
                ApiMac_capabilityInfo_t *pCapInfo)
{
    ApiMac_assocStatus_t status;

    /* Make sure the device is in our PAN */
    if(pDevInfo->panID == devicePanId)
    {
        /* Update the user that a device is joining */
        status = Csf_deviceUpdate(pDevInfo, pCapInfo);
        if(status==ApiMac_assocStatus_success)
        {
#ifdef FEATURE_MAC_SECURITY
            /* Add device to security device table */
            Cllc_addSecDevice(pDevInfo->panID,
                              pDevInfo->shortAddress,
                              &pDevInfo->extAddress, 0);
#endif /* FEATURE_MAC_SECURITY */

#ifdef FEATURE_SECURE_COMMISSIONING

            /* Strictly do not set the event for sending collector
             * config event as this has to be sent only after the
             * secure commissioning is finished successfully
             */

#else
            /* Set event for sending collector config packet */
            Util_setEvent(&Collector_events, COLLECTOR_CONFIG_EVT);
#endif
        }
    }
    else
    {
        status = ApiMac_assocStatus_panAccessDenied;
    }
    return (status);
}

/*!
 * @brief     CLLC State Changed callback.
 *
 * @param     state - CLLC new state
 */
static void cllcStateChangedCB(Cllc_states_t state)
{
    /* Save the state */
    cllcState = state;

    /* Notify the user interface */
    Csf_stateChangeUpdate(cllcState);
}

/*!
 * @brief      MAC Data Confirm callback.
 *
 * @param      pDataCnf - pointer to the data confirm information
 */
static void dataCnfCB(ApiMac_mcpsDataCnf_t *pDataCnf)
{
    /* Record statistics */
    if(pDataCnf->status == ApiMac_status_channelAccessFailure)
    {
        Collector_statistics.channelAccessFailures++;
    }
    else if(pDataCnf->status == ApiMac_status_noAck)
    {
        Collector_statistics.ackFailures++;
    }
    else if(pDataCnf->status == ApiMac_status_transactionExpired)
    {
        Collector_statistics.txTransactionExpired++;
    }
    else if(pDataCnf->status == ApiMac_status_transactionOverflow)
    {
        Collector_statistics.txTransactionOverflow++;
    }
    else if(pDataCnf->status == ApiMac_status_success)
    {
        Csf_updateFrameCounter(NULL, pDataCnf->frameCntr);
    }
    else if(pDataCnf->status != ApiMac_status_success)
    {
        Collector_statistics.otherTxFailures++;
    }

#ifdef POWER_MEAS
    /* Back to back data messages to ensure a response for every poll message */
    if((pDataCnf->msduHandle & RAMP_DATA_MSDU_HANDLE) &&
       (POWER_TEST_PROFILE == POLL_DATA))
    {
        generateIndirectRampMsg();
    }
#endif

    /* Make sure the message came from the app */
    if(pDataCnf->msduHandle & APP_MARKER_MSDU_HANDLE)
    {
        /* What message type was the original request? */
        if(pDataCnf->msduHandle & APP_CONFIG_MSDU_HANDLE)
        {
            /* Config Request */
            Cllc_associated_devices_t *pDev;
            pDev = findDeviceStatusBit(ASSOC_CONFIG_MASK, ASSOC_CONFIG_SENT);
            if(pDev != NULL)
            {
                if(pDataCnf->status != ApiMac_status_success)
                {
                    /* Try to send again */
                    pDev->status &= ~ASSOC_CONFIG_SENT;
                    Csf_setConfigClock(CONFIG_DELAY);
                }
                else
                {
                    pDev->status |= ASSOC_CONFIG_SENT;
                    pDev->status |= ASSOC_CONFIG_RSP;
                    pDev->status |= CLLC_ASSOC_STATUS_ALIVE;
                    Csf_setConfigClock(CONFIG_RESPONSE_DELAY);
                }
            }

            /* Update stats */
            if(pDataCnf->status == ApiMac_status_success)
            {
                Collector_statistics.configReqRequestSent++;
            }
        }
        else if(pDataCnf->msduHandle & APP_BROADCAST_MSDU_HANDLE)
        {
            if(pDataCnf->status == ApiMac_status_success)
            {
                Collector_statistics.broadcastMsgSentCnt++;
            }
        }
        else
        {
            /* Tracking Request */
            Cllc_associated_devices_t *pDev;
            pDev = findDeviceStatusBit(ASSOC_TRACKING_SENT,
                                       ASSOC_TRACKING_SENT);
            if(pDev != NULL)
            {
                if(pDataCnf->status == ApiMac_status_success)
                {
                    /* Make sure the retry is clear */
                    pDev->status &= ~ASSOC_TRACKING_RETRY;
                }
                else
                {
                    if(pDev->status & ASSOC_TRACKING_RETRY)
                    {
                        /* We already tried to resend */
                        pDev->status &= ~ASSOC_TRACKING_RETRY;
                        pDev->status |= ASSOC_TRACKING_ERROR;
                    }
                    else
                    {
                        /* Go ahead and retry */
                        pDev->status |= ASSOC_TRACKING_RETRY;
                    }

                    pDev->status &= ~ASSOC_TRACKING_SENT;

                    /* Try to send again or another */
                    Csf_setTrackingClock(TRACKING_CNF_DELAY_TIME);
                }
            }

            /* Update stats */
            if(pDataCnf->status == ApiMac_status_success)
            {
                Collector_statistics.trackingReqRequestSent++;
            }
        }
    }
}

/*!
 * @brief      MAC Data Indication callback.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void dataIndCB(ApiMac_mcpsDataInd_t *pDataInd)
{
    if((pDataInd != NULL) && (pDataInd->msdu.p != NULL)
       && (pDataInd->msdu.len > 0))
    {
        Smsgs_cmdIds_t cmdId = (Smsgs_cmdIds_t)*(pDataInd->msdu.p);

#ifdef FEATURE_MAC_SECURITY
        {
            if(Cllc_securityCheck(&(pDataInd->sec)) == false)
            {
                /* Reject the message */
                return;
            }
        }
#endif /* FEATURE_MAC_SECURITY */

        if(pDataInd->srcAddr.addrMode == ApiMac_addrType_extended)
        {
            uint16_t shortAddr = Csf_getDeviceShort(
                            &pDataInd->srcAddr.addr.extAddr);
            if(shortAddr != CSF_INVALID_SHORT_ADDR)
            {
                /* Switch to the short address for internal tracking */
                pDataInd->srcAddr.addrMode = ApiMac_addrType_short;
                pDataInd->srcAddr.addr.shortAddr = shortAddr;
            }
            else
            {
                /* Can't accept the message - ignore it */
                return;
            }
        }

        switch(cmdId)
        {
            case Smsgs_cmdIds_configRsp:
                processConfigResponse(pDataInd);
                break;

            case Smsgs_cmdIds_trackingRsp:
                processTrackingResponse(pDataInd);
                break;

            case Smsgs_cmdIds_IdentifyLedReq:
                processIdendifyLedRequest(pDataInd);
                break;

            case Smsgs_cmdIds_toggleLedRsp:
                processToggleLedResponse(pDataInd);
                break;

            case Smsgs_cmdIds_sensorData:
                processSensorData(pDataInd);
                break;
            case Smsgs_cmdIds_rampdata:
                Collector_statistics.sensorMessagesReceived++;
                break;
#ifdef FEATURE_SECURE_COMMISSIONING
            case Smgs_cmdIds_CommissionMsg:
                /* Process Security manager commissioning data */
                SM_processCommData(pDataInd);
                break;
#endif /* FEATURE_SECURE_COMMISSIONING */

            default:
                /* Should not receive other messages */
                break;
        }
    }
}

/*!
 * @brief      Process the start event
 */
static void processStartEvent(void)
{
    Llc_netInfo_t netInfo;
    uint32_t frameCounter = 0;

    Csf_getFrameCounter(NULL, &frameCounter);
    /* See if there is existing network information */
    if(Csf_getNetworkInformation(&netInfo))
    {
        uint16_t numDevices = 0;

#ifdef FEATURE_MAC_SECURITY
        /* Initialize the MAC Security */
        Cllc_securityInit(frameCounter);
#endif /* FEATURE_MAC_SECURITY */

        numDevices = Csf_getNumDeviceListEntries();

        /* Restore with the network and device information */
        Cllc_restoreNetwork(&netInfo, (uint8_t)numDevices, NULL);

        restarted = true;
#ifdef FEATURE_SECURE_COMMISSIONING
        if((numDevices >0 )&&(!CONFIG_FH_ENABLE))
        {
            fCommissionRequired = true;
            keyRecoverDeviceNumber = numDevices;
        }
#endif
    }
    else
    {
        restarted = false;

#ifdef FEATURE_MAC_SECURITY
        /* Initialize the MAC Security */
        Cllc_securityInit(frameCounter);
#endif /* FEATURE_MAC_SECURITY */

        /* Start a new netork */
        Cllc_startNetwork();
    }
}

/*!
 * @brief      Process the Config Response message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processConfigResponse(ApiMac_mcpsDataInd_t *pDataInd)
{
    /* Make sure the message is the correct size */
    if(pDataInd->msdu.len == SMSGS_CONFIG_RESPONSE_MSG_LENGTH)
    {
        Cllc_associated_devices_t *pDev;
        Smsgs_configRspMsg_t configRsp;
        uint8_t *pBuf = pDataInd->msdu.p;

        /* Parse the message */
        configRsp.cmdId = (Smsgs_cmdIds_t)*pBuf++;

        configRsp.status = (Smsgs_statusValues_t)Util_buildUint16(pBuf[0],
                                                                  pBuf[1]);
        pBuf += 2;

        configRsp.frameControl = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;

        configRsp.reportingInterval = Util_buildUint32(pBuf[0], pBuf[1],
                                                       pBuf[2],
                                                       pBuf[3]);
        pBuf += 4;

        configRsp.pollingInterval = Util_buildUint32(pBuf[0], pBuf[1], pBuf[2],
                                                     pBuf[3]);

        pDev = findDevice(&pDataInd->srcAddr);
        if(pDev != NULL)
        {
            /* Clear the sent flag and set the response flag */
            pDev->status &= ~ASSOC_CONFIG_SENT;
            pDev->status |= ASSOC_CONFIG_RSP;
        }

        /* Report the config response */
        Csf_deviceConfigUpdate(&pDataInd->srcAddr, pDataInd->rssi,
                               &configRsp);

        Util_setEvent(&Collector_events, COLLECTOR_CONFIG_EVT);

        Collector_statistics.configResponseReceived++;
    }
}

/*!
 * @brief      Process the Tracking Response message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processTrackingResponse(ApiMac_mcpsDataInd_t *pDataInd)
{
    /* Make sure the message is the correct size */
    if(pDataInd->msdu.len == SMSGS_TRACKING_RESPONSE_MSG_LENGTH)
    {
        Cllc_associated_devices_t *pDev;

        pDev = findDevice(&pDataInd->srcAddr);
        if(pDev != NULL)
        {
            if(pDev->status & ASSOC_TRACKING_SENT)
            {
                pDev->status &= ~ASSOC_TRACKING_SENT;
                pDev->status |= ASSOC_TRACKING_RSP;

                /* Setup for next tracking */
                Csf_setTrackingClock( TRACKING_DELAY_TIME);

                /* Retry config request */
                processConfigRetry();
            }
        }

        /* Update stats */
        Collector_statistics.trackingResponseReceived++;
    }
}

/*!
 * @brief      Process the Toggle Led Response message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processIdendifyLedRequest(ApiMac_mcpsDataInd_t *pDataInd)
{
    /* Make sure the message is the correct size */
    if(pDataInd->msdu.len == SMSGS_INDENTIFY_LED_REQUEST_MSG_LEN)
    {
        Llc_deviceListItem_t item;

        /* Is the device a known device? */
        if(Csf_getDevice(&(pDataInd->srcAddr), &item))
        {
            uint8_t cmdBytes[SMSGS_INDENTIFY_LED_RESPONSE_MSG_LEN];
            Csf_identifyLED((pDataInd->msdu.p[1]) * 1000);

            /* send the response message directly */
            cmdBytes[0] = (uint8_t) Smsgs_cmdIds_IdentifyLedRsp;
            cmdBytes[1] = 0;
            sendMsg(Smsgs_cmdIds_toggleLedRsp,
                    item.devInfo.shortAddress,
                    item.capInfo.rxOnWhenIdle,
                    SMSGS_INDENTIFY_LED_RESPONSE_MSG_LEN,
                    cmdBytes);
        }
    }
}

/*!
 * @brief      Process the Toggle Led Response message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processToggleLedResponse(ApiMac_mcpsDataInd_t *pDataInd)
{
    /* Make sure the message is the correct size */
    if(pDataInd->msdu.len == SMSGS_TOGGLE_LED_RESPONSE_MSG_LEN)
    {
        bool ledState;
        uint8_t *pBuf = pDataInd->msdu.p;

        /* Skip past the command ID */
        pBuf++;

        ledState = (bool)*pBuf;

        /* Notify the user */
        Csf_toggleResponseReceived(&pDataInd->srcAddr, ledState);
    }
}

/*!
 * @brief      Process the Sensor Data message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processSensorData(ApiMac_mcpsDataInd_t *pDataInd)
{
    Smsgs_sensorMsg_t sensorData;
    uint8_t *pBuf = pDataInd->msdu.p;

    memset(&sensorData, 0, sizeof(Smsgs_sensorMsg_t));

    /* Parse the message */
    sensorData.cmdId = (Smsgs_cmdIds_t)*pBuf++;

    memcpy(sensorData.extAddress, pBuf, SMGS_SENSOR_EXTADDR_LEN);
    pBuf += SMGS_SENSOR_EXTADDR_LEN;

    sensorData.frameControl = Util_buildUint16(pBuf[0], pBuf[1]);
    pBuf += 2;

    /* Parse data in order of frameControl mask, starting with LSB */
    if(sensorData.frameControl & Smsgs_dataFields_tempSensor)
    {
        sensorData.tempSensor.ambienceTemp = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.tempSensor.objectTemp = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }

    if(sensorData.frameControl & Smsgs_dataFields_lightSensor)
    {
        sensorData.lightSensor.rawData = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }

    if(sensorData.frameControl & Smsgs_dataFields_humiditySensor)
    {
        sensorData.humiditySensor.temp = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.humiditySensor.humidity = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }

    if(sensorData.frameControl & Smsgs_dataFields_msgStats)
    {
        sensorData.msgStats.joinAttempts = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.joinFails = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.msgsAttempted = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.msgsSent = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.trackingRequests = Util_buildUint16(pBuf[0],
                                                                pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.trackingResponseAttempts = Util_buildUint16(
                        pBuf[0],
                        pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.trackingResponseSent = Util_buildUint16(pBuf[0],
                                                                    pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.configRequests = Util_buildUint16(pBuf[0],
                                                              pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.configResponseAttempts = Util_buildUint16(
                        pBuf[0],
                        pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.configResponseSent = Util_buildUint16(pBuf[0],
                                                                  pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.channelAccessFailures = Util_buildUint16(pBuf[0],
                                                                     pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.macAckFailures = Util_buildUint16(pBuf[0], pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.otherDataRequestFailures = Util_buildUint16(
                        pBuf[0],
                        pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.syncLossIndications = Util_buildUint16(pBuf[0],
                                                                   pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.rxDecryptFailures = Util_buildUint16(pBuf[0],
                                                                 pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.txEncryptFailures = Util_buildUint16(pBuf[0],
                                                                 pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.resetCount = Util_buildUint16(pBuf[0],
                                                          pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.lastResetReason = Util_buildUint16(pBuf[0],
                                                               pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.joinTime = Util_buildUint16(pBuf[0],
                                                        pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.interimDelay = Util_buildUint16(pBuf[0],
                                                            pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.numBroadcastMsgRcvd = Util_buildUint16(pBuf[0],
                                                                   pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.numBroadcastMsglost = Util_buildUint16(pBuf[0],
                                                                   pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.avgE2EDelay = Util_buildUint16(pBuf[0],pBuf[1]);
        pBuf += 2;
        sensorData.msgStats.worstCaseE2EDelay = Util_buildUint16(pBuf[0],pBuf[1]);
        pBuf += 2;
    }

    if(sensorData.frameControl & Smsgs_dataFields_configSettings)
    {
        sensorData.configSettings.reportingInterval = Util_buildUint32(pBuf[0],
                                                                       pBuf[1],
                                                                       pBuf[2],
                                                                       pBuf[3]);
        pBuf += 4;
        sensorData.configSettings.pollingInterval = Util_buildUint32(pBuf[0],
                                                                     pBuf[1],
                                                                     pBuf[2],
                                                                     pBuf[3]);
    }

    Collector_statistics.sensorMessagesReceived++;

    /* Report the sensor data */
    Csf_deviceSensorDataUpdate(&pDataInd->srcAddr, pDataInd->rssi,
                               &sensorData);

    processDataRetry(&(pDataInd->srcAddr));
}

/*!
 * @brief      Find the associated device table entry matching pAddr.
 *
 * @param      pAddr - pointer to device's address
 *
 * @return     pointer to the associated device table entry,
 *             NULL if not found.
 */
static Cllc_associated_devices_t *findDevice(ApiMac_sAddr_t *pAddr)
{
    int x;
    Cllc_associated_devices_t *pItem = NULL;

    /* Check for invalid parameters */
    if((pAddr == NULL) || (pAddr->addrMode == ApiMac_addrType_none))
    {
        return (NULL);
    }

    for(x = 0; x < CONFIG_MAX_DEVICES; x++)
    {
        /* Make sure the entry is valid. */
        if(Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
        {
            if(pAddr->addrMode == ApiMac_addrType_short)
            {
                if(pAddr->addr.shortAddr == Cllc_associatedDevList[x].shortAddr)
                {
                    pItem = &Cllc_associatedDevList[x];
                    break;
                }
            }
        }
    }

    return (pItem);
}

/*!
 * @brief      Find the associated device table entry matching status bit.
 *
 * @param      statusBit - what status bit to find
 *
 * @return     pointer to the associated device table entry,
 *             NULL if not found.
 */
static Cllc_associated_devices_t *findDeviceStatusBit(uint16_t mask, uint16_t statusBit)
{
    int x;
    Cllc_associated_devices_t *pItem = NULL;

    for(x = 0; x < CONFIG_MAX_DEVICES; x++)
    {
        /* Make sure the entry is valid. */
        if(Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
        {
            if((Cllc_associatedDevList[x].status & mask) == statusBit)
            {
                pItem = &Cllc_associatedDevList[x];
                break;
            }
        }
    }

    return (pItem);
}

/*!
 * @brief      Get the next MSDU Handle
 *             <BR>
 *             The MSDU handle has 3 parts:<BR>
 *             - The MSBit(7), when set means the the application sent the message
 *             - Bit 6, when set means that the app message is a config request
 *             - Bits 0-5, used as a message counter that rolls over.
 *
 * @param      msgType - message command id needed
 *
 * @return     msdu Handle
 */
static uint8_t getMsduHandle(Smsgs_cmdIds_t msgType)
{
    uint8_t msduHandle = deviceTxMsduHandle;

    /* Increment for the next msdu handle, or roll over */
    if(deviceTxMsduHandle >= MSDU_HANDLE_MAX)
    {
        deviceTxMsduHandle = 0;
    }
    else
    {
        deviceTxMsduHandle++;
    }

    /* Add the message type bit for ramp data */
    if(msgType == Smsgs_cmdIds_rampdata)
    {
        msduHandle |= RAMP_DATA_MSDU_HANDLE;
        return (msduHandle);
    }

    /* Add the App specific bit */
    msduHandle |= APP_MARKER_MSDU_HANDLE;

    /* Add the message type bit */
    if(msgType == Smsgs_cmdIds_configReq)
    {
        msduHandle |= APP_CONFIG_MSDU_HANDLE;
    }
    else if(msgType == Smgs_cmdIds_broadcastCtrlMsg)
    {
        msduHandle |= APP_BROADCAST_MSDU_HANDLE;
    }

    return (msduHandle);
}

/*!
 * @brief      Send MAC data request
 *
 * @param      type - message type
 * @param      dstShortAddr - destination short address
 * @param      rxOnIdle - true if not a sleepy device
 * @param      len - length of payload
 * @param      pData - pointer to the buffer
 *
 * @return  true if sent, false if not
 */
static bool sendMsg(Smsgs_cmdIds_t type, uint16_t dstShortAddr, bool rxOnIdle,
                    uint16_t len,
                    uint8_t *pData)
{
    ApiMac_mcpsDataReq_t dataReq;

    /* Fill the data request field */
    memset(&dataReq, 0, sizeof(ApiMac_mcpsDataReq_t));

    dataReq.dstAddr.addrMode = ApiMac_addrType_short;
    dataReq.dstAddr.addr.shortAddr = dstShortAddr;
    dataReq.srcAddrMode = ApiMac_addrType_short;

    if(fhEnabled && rxOnIdle)
    {
        Llc_deviceListItem_t item;

        if(Csf_getDevice(&(dataReq.dstAddr), &item))
        {
            /* Switch to the long address */
            dataReq.dstAddr.addrMode = ApiMac_addrType_extended;
            memcpy(&dataReq.dstAddr.addr.extAddr, &item.devInfo.extAddress,
                   (APIMAC_SADDR_EXT_LEN));
            dataReq.srcAddrMode = ApiMac_addrType_extended;
        }
        else
        {
            /* Can't send the message */
            return (false);
        }
    }

    dataReq.dstPanId = devicePanId;

    dataReq.msduHandle = getMsduHandle(type);

    dataReq.txOptions.ack = true;
    if(rxOnIdle == false)
    {
        dataReq.txOptions.indirect = true;
    }

    dataReq.msdu.len = len;
    dataReq.msdu.p = pData;

#ifdef FEATURE_MAC_SECURITY
#ifdef FEATURE_SECURE_COMMISSIONING
    {
        Llc_deviceListItem_t deviceItem;

        if(Csf_getDevice(&(dataReq.dstAddr), &deviceItem)) {
            SM_getSrcDeviceSecurityInfo(deviceItem.devInfo.extAddress,deviceItem.devInfo.shortAddress, &dataReq.sec);
        }
        else {
            /* Can't send the message */
            return (false);
        }
    }
#else
    /* Fill in the appropriate security fields */
    Cllc_securityFill(&dataReq.sec);
#endif /* FEATURE_SECURE_COMMISSIONING */
#endif /* FEATURE_MAC_SECURITY */

    /* Send the message */
    if(ApiMac_mcpsDataReq(&dataReq) != ApiMac_status_success)
    {
        /*  Transaction overflow occurred */
        return (false);
    }
    else
    {
        return (true);
    }
}

/*!
 * @brief      Send MAC broadcast data request. Only supported in FH mode.
 *
 * @param      type - message type
 * @param      len - length of payload
 * @param      pData - pointer to the buffer
 */
static void sendBroadcastMsg(Smsgs_cmdIds_t type, uint16_t len,
                    uint8_t *pData)
{
    ApiMac_mcpsDataReq_t dataReq;

    /* Only supported for FH mode */
    if(!fhEnabled)
    {
        return;
    }
    /* Fill the data request field */
    memset(&dataReq, 0, sizeof(ApiMac_mcpsDataReq_t));

    dataReq.dstAddr.addrMode = ApiMac_addrType_none;
    dataReq.srcAddrMode = ApiMac_addrType_short;

    dataReq.dstPanId = devicePanId;

    dataReq.msduHandle = getMsduHandle(type);

    dataReq.txOptions.ack = false;
    dataReq.txOptions.indirect = false;


    dataReq.msdu.len = len;
    dataReq.msdu.p = pData;

#ifdef FEATURE_MAC_SECURITY
    /* Fill in the appropriate security fields */
    Cllc_securityFill(&dataReq.sec);
#endif /* FEATURE_MAC_SECURITY */

    /* Send the message */
    ApiMac_mcpsDataReq(&dataReq);
}
/*!
 @brief  Build and send fixed size ramp data to the first associated device.
 */
#ifdef POWER_MEAS
void generateIndirectRampMsg()
{
    uint8_t *pMsgBuf;
    uint16_t index;
    uint16_t sensorShortAddr;

    sensorShortAddr = Cllc_associatedDevList[0].shortAddr;
    if ((sensorShortAddr == CSF_INVALID_SHORT_ADDR) ||
        (!COLLECTOR_TEST_RAMP_DATA_SIZE))
    {
        return;
    }

    pMsgBuf = (uint8_t *)Csf_malloc(COLLECTOR_TEST_RAMP_DATA_SIZE);
    if(pMsgBuf)
    {
        uint8_t *pBuf = pMsgBuf;
        *pBuf++ = (uint8_t)Smsgs_cmdIds_rampdata;
        for(index = 1; index < COLLECTOR_TEST_RAMP_DATA_SIZE; index++)
        {
            *pBuf++ = (uint8_t) (index & 0xFF);
        }

        sendMsg(Smsgs_cmdIds_rampdata, sensorShortAddr, false,
                COLLECTOR_TEST_RAMP_DATA_SIZE, pMsgBuf);

        Csf_free(pMsgBuf);
    }

}
#endif

/*!
 * @brief      Generate Config Requests for all associate devices
 *             that need one.
 */
static void generateConfigRequests(void)
{
#ifndef POWER_MEAS
    int x;

    if(CERTIFICATION_TEST_MODE)
    {
        /* In Certification mode only back to back uplink
         * data traffic shall be supported*/
        return;
    }

    /* Clear any timed out transactions */
    for(x = 0; x < CONFIG_MAX_DEVICES; x++)
    {
        if((Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
           && (Cllc_associatedDevList[x].status & CLLC_ASSOC_STATUS_ALIVE))
        {
            if((Cllc_associatedDevList[x].status &
               (ASSOC_CONFIG_SENT | ASSOC_CONFIG_RSP))
               == (ASSOC_CONFIG_SENT | ASSOC_CONFIG_RSP))
            {
                Cllc_associatedDevList[x].status &= ~(ASSOC_CONFIG_SENT
                                | ASSOC_CONFIG_RSP);
            }
        }
    }

    /* Make sure we are only sending one config request at a time */
    if(findDeviceStatusBit(ASSOC_CONFIG_MASK, ASSOC_CONFIG_SENT) == NULL)
    {
        /* Run through all of the devices */
        for(x = 0; x < CONFIG_MAX_DEVICES; x++)
        {
            /* Make sure the entry is valid. */
            if((Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
               && (Cllc_associatedDevList[x].status & CLLC_ASSOC_STATUS_ALIVE))
            {
                uint16_t status = Cllc_associatedDevList[x].status;

                /*
                 Has the device been sent or already received a config request?
                 */
                if(((status & (ASSOC_CONFIG_SENT | ASSOC_CONFIG_RSP)) == 0))
                {
                    ApiMac_sAddr_t dstAddr;
                    Collector_status_t stat;

                    /* Set up the destination address */
                    dstAddr.addrMode = ApiMac_addrType_short;
                    dstAddr.addr.shortAddr =
                        Cllc_associatedDevList[x].shortAddr;

                    /* Send the Config Request */
                    stat = Collector_sendConfigRequest(
                                    &dstAddr, (CONFIG_FRAME_CONTROL),
                                    (CONFIG_REPORTING_INTERVAL),
                                    (CONFIG_POLLING_INTERVAL));
                    if(stat == Collector_status_success)
                    {
                        /*
                         Mark as the message has been sent and expecting a response
                         */
                        Cllc_associatedDevList[x].status |= ASSOC_CONFIG_SENT;
                        Cllc_associatedDevList[x].status &= ~ASSOC_CONFIG_RSP;
                    }

                    /* Only do one at a time */
                    break;
                }
            }
        }
    }
#endif
}


/*!
 * @brief      Generate Tracking Requests for all associate devices
 *             that need one.
 */
static void generateTrackingRequests(void)
{
#ifndef POWER_MEAS
    int x;

    /* Run through all of the devices, looking for previous activity */
    for(x = 0; x < CONFIG_MAX_DEVICES; x++)
    {
        if(CERTIFICATION_TEST_MODE)
        {
            /* In Certification mode only back to back uplink
             * data traffic shall be supported*/
            return;
        }
        /* Make sure the entry is valid. */
        if((Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
             && (Cllc_associatedDevList[x].status & CLLC_ASSOC_STATUS_ALIVE))
        {
            uint16_t status = Cllc_associatedDevList[x].status;

            /*
             Has the device been sent a tracking request or received a
             tracking response?
             */
            if(status & ASSOC_TRACKING_RETRY)
            {
                sendTrackingRequest(&Cllc_associatedDevList[x]);
                return;
            }
            else if((status & (ASSOC_TRACKING_SENT | ASSOC_TRACKING_RSP
                               | ASSOC_TRACKING_ERROR)))
            {
                Cllc_associated_devices_t *pDev = NULL;
                int y;

                if(status & (ASSOC_TRACKING_SENT | ASSOC_TRACKING_ERROR))
                {
                    ApiMac_deviceDescriptor_t devInfo;
                    Llc_deviceListItem_t item;
                    ApiMac_sAddr_t devAddr;

                    /*
                     Timeout occured, notify the user that the tracking
                     failed.
                     */
                    memset(&devInfo, 0, sizeof(ApiMac_deviceDescriptor_t));

                    devAddr.addrMode = ApiMac_addrType_short;
                    devAddr.addr.shortAddr =
                        Cllc_associatedDevList[x].shortAddr;

                    if(Csf_getDevice(&devAddr, &item))
                    {
                        memcpy(&devInfo.extAddress,
                               &item.devInfo.extAddress,
                               sizeof(ApiMac_sAddrExt_t));
                    }
                    devInfo.shortAddress = Cllc_associatedDevList[x].shortAddr;
                    devInfo.panID = devicePanId;
                    Csf_deviceNotActiveUpdate(&devInfo,
                        ((status & ASSOC_TRACKING_SENT) ? true : false));

                    /* Not responding, so remove the alive marker */
                    Cllc_associatedDevList[x].status
                            &= ~(CLLC_ASSOC_STATUS_ALIVE
                                | ASSOC_CONFIG_SENT | ASSOC_CONFIG_RSP);
                }

                /* Clear the tracking bits */
                Cllc_associatedDevList[x].status  &= ~(ASSOC_TRACKING_ERROR
                                | ASSOC_TRACKING_SENT | ASSOC_TRACKING_RSP);

                /* Find the next valid device */
                y = x;
                while(pDev == NULL)
                {
                    /* Check for rollover */
                    if(y == (CONFIG_MAX_DEVICES-1))
                    {
                        /* Move to the beginning */
                        y = 0;
                    }
                    else
                    {
                        /* Move the the next device */
                        y++;
                    }

                    if(y == x)
                    {
                        /* We've come back around */
                        break;
                    }

                    /*
                     Is the entry valid and active */
                    if((Cllc_associatedDevList[y].shortAddr
                                    != CSF_INVALID_SHORT_ADDR)
                         && (Cllc_associatedDevList[y].status
                                   & CLLC_ASSOC_STATUS_ALIVE))
                    {
                        pDev = &Cllc_associatedDevList[y];
                    }
                }

                if(pDev == NULL)
                {
                    /* Another device wasn't found, send to same device */
                    pDev = &Cllc_associatedDevList[x];
                }

                sendTrackingRequest(pDev);

                /* Only do one at a time */
                return;
            }
        }
    }

    /* If no activity found, find the first active device */
    for(x = 0; x < CONFIG_MAX_DEVICES; x++)
    {
        /* Make sure the entry is valid. */
        if((Cllc_associatedDevList[x].shortAddr != CSF_INVALID_SHORT_ADDR)
              && (Cllc_associatedDevList[x].status & CLLC_ASSOC_STATUS_ALIVE))
        {
            sendTrackingRequest(&Cllc_associatedDevList[x]);
            break;
        }
    }

    if(x == CONFIG_MAX_DEVICES)
    {
        /* No device found, Setup delay for next tracking message */
        Csf_setTrackingClock(TRACKING_DELAY_TIME);
    }
#endif
}

/*!
 * @brief      Generate Broadcast Cmd Request Message
 */
static void generateBroadcastCmd(void)
{
    uint8_t buffer[SMSGS_BROADCAST_CMD_LENGTH];
    uint8_t *pBuf = buffer;

    /* Build the message */
    *pBuf++ = (uint8_t)Smgs_cmdIds_broadcastCtrlMsg;
    *pBuf++ = Util_loUint16(Collector_statistics.broadcastMsgSentCnt);
    *pBuf++ = Util_hiUint16(Collector_statistics.broadcastMsgSentCnt);

    sendBroadcastMsg(Smgs_cmdIds_broadcastCtrlMsg, SMSGS_BROADCAST_CMD_LENGTH,
                     buffer);
}

/*!
 * @brief      Generate Tracking Requests for a device
 *
 * @param      pDev - pointer to the device's associate device table entry
 */
static void sendTrackingRequest(Cllc_associated_devices_t *pDev)
{
    uint8_t cmdId = Smsgs_cmdIds_trackingReq;

    /* Send the Tracking Request */
   if((sendMsg(Smsgs_cmdIds_trackingReq, pDev->shortAddr,
            pDev->capInfo.rxOnWhenIdle,
            (SMSGS_TRACKING_REQUEST_MSG_LENGTH),
            &cmdId)) == true)
    {
        /* Mark as Tracking Request sent */
        pDev->status |= ASSOC_TRACKING_SENT;

        /* Setup Timeout for response */
        Csf_setTrackingClock(TRACKING_TIMEOUT_TIME);

        /* Update stats */
        Collector_statistics.trackingRequestAttempts++;
    }
    else
    {
        ApiMac_sAddr_t devAddr;
        devAddr.addrMode = ApiMac_addrType_short;
        devAddr.addr.shortAddr = pDev->shortAddr;
        processDataRetry(&devAddr);
    }
}

/*!
 * @brief      Process the MAC Comm Status Indication Callback
 *
 * @param      pCommStatusInd - Comm Status indication
 */
static void commStatusIndCB(ApiMac_mlmeCommStatusInd_t *pCommStatusInd)
{
    if(pCommStatusInd->reason == ApiMac_commStatusReason_assocRsp)
    {
        if(pCommStatusInd->status != ApiMac_status_success)
        {
            Cllc_associated_devices_t *pDev;

            pDev = findDevice(&pCommStatusInd->dstAddr);
            if(pDev)
            {
                /* Mark as inactive and clear config and tracking states */
                pDev->status = 0;
            }
        }
    }
}

/*!
 * @brief      Process the MAC Poll Indication Callback
 *
 * @param      pPollInd - poll indication
 */
static void pollIndCB(ApiMac_mlmePollInd_t *pPollInd)
{
    ApiMac_sAddr_t addr;

    addr.addrMode = ApiMac_addrType_short;
    if (pPollInd->srcAddr.addrMode == ApiMac_addrType_short)
    {
        addr.addr.shortAddr = pPollInd->srcAddr.addr.shortAddr;
    }
    else
    {
        addr.addr.shortAddr = Csf_getDeviceShort(
                        &pPollInd->srcAddr.addr.extAddr);
    }

    processDataRetry(&addr);
}

/*!
 * @brief      Process retries for config and tracking messages
 *
 * @param      addr - MAC address structure */
static void processDataRetry(ApiMac_sAddr_t *pAddr)
{
    if(pAddr->addr.shortAddr != CSF_INVALID_SHORT_ADDR)
    {
        Cllc_associated_devices_t *pItem;
        pItem = findDevice(pAddr);
        if(pItem)
        {
            /* Set device status to alive */
            pItem->status |= CLLC_ASSOC_STATUS_ALIVE;

            /* Check to see if we need to send it a config */
            if((pItem->status & (ASSOC_CONFIG_RSP | ASSOC_CONFIG_SENT)) == 0)
            {
                processConfigRetry();
            }
            /* Check to see if we need to send it a tracking message */
            if((pItem->status & (ASSOC_TRACKING_SENT| ASSOC_TRACKING_RETRY)) == 0)
            {
                /* Make sure we aren't already doing a tracking message */
                if(((Collector_events & COLLECTOR_TRACKING_TIMEOUT_EVT) == 0)
                    && (Csf_isTrackingTimerActive() == false)
                    && (findDeviceStatusBit(ASSOC_TRACKING_MASK,
                                            ASSOC_TRACKING_SENT) == NULL))
                {
                    /* Setup for next tracking */
                    Csf_setTrackingClock(TRACKING_DELAY_TIME);
                }
            }
        }
    }
}

/*!
 * @brief      Process retries for config messages
 */
static void processConfigRetry(void)
{
    /* Retry config request if not already sent */
    if(((Collector_events & COLLECTOR_CONFIG_EVT) == 0)
        && (Csf_isConfigTimerActive() == false))
    {
        /* Set config event */
        Csf_setConfigClock(CONFIG_DELAY);
    }
}

#ifdef FEATURE_SECURE_COMMISSIONING
/*!
 * @brief      Security manager failure processing function
 */
void smFailCMProcessCb(ApiMac_deviceDescriptor_t *devInfo, bool rxOnIdle,
                       bool keyRefreshment)
{
    /* Re-enable joining if it was enabled prior to key refreshment process, or
     * if commissioning was a result of device association as network must have
     * been open for association to occur
     */
    if (keyRefreshment) {

        if (permitJoining) {
            // the permitJoining was allowed before, so let's allow it
            uint32_t duration = 0xFFFFFFFF;
            Cllc_setJoinPermit(duration);
        }
    }
    else if (fCommissionRequired  == false) {
        // secure commissioning. The permitJoining was allowed before, so let's allow it
        uint32_t duration = 0xFFFFFFFF;
        Cllc_setJoinPermit(duration);
    }


    if (keyRefreshment == true)
    {   /* Device failed key refreshment process */
        LCD_WRITE_STRING_VALUE("Key Refresh Failed: 0x",
                               devInfo->shortAddress, 16, 6);
    }
    else
    {   /* Device failed commissioning process */
        LCD_WRITE_STRING_VALUE("Commissioning Failed: 0x",
                               devInfo->shortAddress, 16, 6);

        /* Send a disassociation request to the device */
        Cllc_sendDisassociationRequest(devInfo->shortAddress, rxOnIdle);
        /* Remove device from the NV list */
        Cllc_removeDevice(&devInfo->extAddress);

        Util_setEvent(&Collector_events, COLLECTOR_CONFIG_EVT);
    }
}

/*!
 * @brief      Security manager success processing function
 */
void smSuccessCMProcessCb(ApiMac_deviceDescriptor_t *devInfo, bool keyRefreshment)
{
    /* Re-enable joining if it was enabled prior to key refreshment process, or
     * if commissioning was a result of device association as network must have
     * been open for association to occur
     */
    if (keyRefreshment) {

        if (permitJoining) {
            // the permitJoining was allowed before, so let's allow it
            uint32_t duration = 0xFFFFFFFF;
            Cllc_setJoinPermit(duration);
        }
    }
    else if (fCommissionRequired  == false) {
        // secure commissioning. The permitJoining was allowed before, so let's allow it
        uint32_t duration = 0xFFFFFFFF;
        Cllc_setJoinPermit(duration);
    }

    if (keyRefreshment == true)
    {   /* Device successfully finished key refreshment process */
        LCD_WRITE_STRING_VALUE("Key Refreshed: 0x",
                               devInfo->shortAddress, 16, 6);
    }
    else
    {   /* Device successfully finished commissioning process */
        LCD_WRITE_STRING_VALUE("Commissioned: 0x",
                               devInfo->shortAddress, 16, 6);

        Util_setEvent(&Collector_events, COLLECTOR_CONFIG_EVT);
    }
}
#endif /* FEATURE_SECURE_COMMISSIONING */

/*!
 * @brief       Process Orphan indication callback
 *
 * @param       pData - pointer to orphan indication callback structure
 */
static void orphanIndCb(ApiMac_mlmeOrphanInd_t *pData)
{
    uint16_t shortAddr = Csf_getDeviceShort(&pData->orphanAddress);
    /* get the short address of the device */
    if(CSF_INVALID_SHORT_ADDR != shortAddr)
    {
        Csf_IndicateOrphanReJoin(shortAddr);
    }

}
