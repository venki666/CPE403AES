
/******************************************************************************

 @file  macTask.c

 @brief Mac Stack interface function implementation

 Group: WCS LPC
 Target Device: cc13x0

 ******************************************************************************
 
 Copyright (c) 2013-2019, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 
 
 *****************************************************************************/

#include "macTask.h"
#include "mac_main.h"
#include "mac_data.h"
#include "mac_pib.h"
#include "macs.h"
#include "mac_mgmt.h"
#include "mac_security.h"
#include "mac_security_pib.h"
#include "hal_mcu.h"
#include "macwrapper.h"

#include "crypto_mac_api.h"
#include <chipinfo.h>
#include <ti/drivers/utils/Random.h>

#include "mac_symbol_timer.h"

#ifdef FEATURE_MAC_SECURITY
#include "hal_aes.h"
#endif

#include "Board.h"

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/dpl/HwiP.h>

#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include "rom_jt.h"

#include <stdint.h>

#include "osal_port.h"
#include "osal_port_timers.h"

// TODO: move this
#define OSALPORT_CLEAN_UP_TIMERS_EVT    0x4000

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

#define MAC_TASK_PRIORITY   3
#define MAC_TASK_STACK_SIZE 1024

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */
Task_Struct macTask;        /* not static so you can see in ROV */
static Task_Handle macTaskHndl;
static Task_Params macTaskParams;
static uint8_t macTaskStack[MAC_TASK_STACK_SIZE];

Semaphore_Struct macSem;  /* not static so you can see in ROV */
static Semaphore_Handle macSemHandle;

static uint32_t macEvents = 0;
static uint32_t macTaskEvents = 0;
static uint8_t _macTaskId;

/* Size table for MAC structures */
const uint8 msacbackSizeTable [] =
{
  0,                                   /* unused */
  sizeof(macMlmeAssociateInd_t),       /* MAC_MLME_ASSOCIATE_IND */\
  sizeof(macMlmeAssociateCnf_t),       /* MAC_MLME_ASSOCIATE_CNF */
  sizeof(macMlmeDisassociateInd_t),    /* MAC_MLME_DISASSOCIATE_IND */
  sizeof(macMlmeDisassociateCnf_t),    /* MAC_MLME_DISASSOCIATE_CNF */
  sizeof(macMlmeBeaconNotifyInd_t),    /* MAC_MLME_BEACON_NOTIFY_IND */
  sizeof(macMlmeOrphanInd_t),          /* MAC_MLME_ORPHAN_IND */
  sizeof(macMlmeScanCnf_t),            /* MAC_MLME_SCAN_CNF */
  sizeof(macMlmeStartCnf_t),           /* MAC_MLME_START_CNF */
  sizeof(macMlmeSyncLossInd_t),        /* MAC_MLME_SYNC_LOSS_IND */
  sizeof(macMlmePollCnf_t),            /* MAC_MLME_POLL_CNF */
  sizeof(macMlmeCommStatusInd_t),      /* MAC_MLME_COMM_STATUS_IND */
  sizeof(macMcpsDataCnf_t),            /* MAC_MCPS_DATA_CNF */
  sizeof(macMcpsDataInd_t),            /* MAC_MCPS_DATA_IND */
  sizeof(macMcpsPurgeCnf_t),           /* MAC_MCPS_PURGE_CNF */
  sizeof(macEventHdr_t),               /* MAC_PWR_ON_CNF */
  sizeof(macMlmePollInd_t),            /* MAC_MLME_POLL_IND */
  sizeof(macMlmeWSAsyncCnf_t),         /* MAC_MLME_WS_ASYNC_CNF */
  sizeof(macMlmeWSAsyncInd_t)          /* MAC_MLME_WS_ASYNC_IND */
};

/* ------------------------------------------------------------------------------------------------
 *                                         Local Variables
 * ------------------------------------------------------------------------------------------------
 */

/* Holds the value for query transmit, passed by the application.
 * A value of 0 indicates no continuation of retry,
 * A value of 1 indicates continue retransmission.
 */
static uint8 queryRetransmit = 0;

/* Holds the value for pending indirect msgs, passed by the application.
 * A value of 0 indicates no pending indirect msg.
 */
static uint8 checkPending = 0;

/* The Application will send the address of the MAC_CbackQueryRetransmit()
 * as part of MAC_INIT message. Save to function pointer below
 */
uint8 (*pMacCbackQueryRetransmit)() = NULL;

/* The Application will send the address of the MAC_CbackCheckPending()
 * as part of MAC_INIT message. Save to function pointer below
 */
uint8 (*pMacCbackCheckPending)() = NULL;

uint8_t AppTaskId;

/* ------------------------------------------------------------------------------------------------
 *                                           Functions
 * ------------------------------------------------------------------------------------------------
 */
static void macApp(macCmd_t *pMsg);
static void macInit(macUserCfg_t *pUserCfg);
static void macTaskFxn(UArg a0, UArg a1);
extern uint8 MAC_MlmeGetReqSize( uint8 pibAttribute );
extern uint8 MAC_MlmeGetSecurityReqSize( uint8 pibAttribute );
extern uint8 MAC_MlmeFHGetReqSize( uint16 pibAttribute );

/**************************************************************************************************
 * @fn          macTaskInit
 *
 * @brief       This function is called initialize the MAC task.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 *
 * @return      MAC Task ID.
 **************************************************************************************************
 */
uint8_t macTaskInit(macUserCfg_t *pUserCfg)
{
    /* create semaphores for messages / events
     */
    Semaphore_Params semParam;
    Semaphore_Params_init(&semParam);
    semParam.mode = Semaphore_Mode_COUNTING;
    Semaphore_construct(&macSem, 0, &semParam);
    macSemHandle = Semaphore_handle(&macSem);

    /* create the Mac Task
     */
    Task_Params_init(&macTaskParams);
    macTaskParams.stackSize = MAC_TASK_STACK_SIZE;
    macTaskParams.priority = MAC_TASK_PRIORITY;
    macTaskParams.stack = &macTaskStack;
    macTaskParams.arg0 = (xdc_UArg) pUserCfg;
    Task_construct(&macTask, macTaskFxn, &macTaskParams, NULL);
    macTaskHndl = Task_handle(&macTask);

#ifdef TIMAC_ROM_IMAGE_BUILD
  TIMAC_ROM_Init();
#endif

    _macTaskId = OsalPort_registerTask(macTaskHndl, macSemHandle, &macTaskEvents);
    macMainSetTaskId(_macTaskId);

    return _macTaskId;
}

/**************************************************************************************************
 * @fn          macTaskGetTaskHndl
 *
 * @brief       This function returns the TIRTOS Task handle of the MAC Task.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 *
 * @return      MAC Task ID.
 **************************************************************************************************
 */
Task_Handle* macTaskGetTaskHndl(void)
{
    return &macTaskHndl;
}

/**************************************************************************************************
 * @fn          macInit
 *
 * @brief       This function is called to initialize the MAC.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
static void macInit(macUserCfg_t *pUserCfg)
{
  uint32_t key;
  int_fast16_t status;

#ifdef FEATURE_MAC_SECURITY
#if !defined(DeviceFamily_CC13X2) && !defined(DeviceFamily_CC26X2)
  CryptoCC26XX_Params CryptoCC26XXParams;
#elif !defined(USE_DMM)
  AESCCM_Params AESCCMParams;
#endif
#endif //FEATURE_MAC_SECURITY

#if defined( DEBUG_SW_TRACE )
  #if defined(HAL_UART) && (HAL_UART==TRUE)
    // Enable tracer output on DIO24/ATEST1.
    // Set max drive strength and reduced slew rate.
    // PortID = 46 = RFcore tracer.
    HWREG(IOC_BASE + IOC_O_IOCFG24) = (3 << IOC_IOCFG24_IOSTR_S) | (1 << IOC_IOCFG24_SLEW_RED_S) | 46;

  #else // no UART so allow trace on Tx pin - Debug Package only

    // Enable tracer output on DIO23/ATEST0 or DIO24/ATEST1:
    // For 7x7 EM on SmartRF06EB:
    // Port ID for RF Tracer is 46
    // [see ioc.h: #define IOC_PORT_RFC_TRC  0x0000002E // RF Core Tracer]

    // DIO 23 maps to IOID 8
    // [see Board.h: #define Board_SPI0_MISO IOID_8     /* RF1.20 */]
    HWREG(IOC_BASE + IOC_O_IOCFG23) = (3 << IOC_IOCFG23_IOSTR_S) | 46;

    // OR

    // DIO 24 maps to IOID 24
    // [see Board.h: #define Board_SPI1_MISO IOID_24   /* RF2.10 */]
    //HWREG(IOC_BASE + IOC_O_IOCFG24) = (3 << IOC_IOCFG24_IOSTR_S) | 46;

  #endif // HAL_UART
#endif // DEBUG_SW_TRACE

  /* User App Reconfiguration of TIMAC */
  macSetUserConfig( pUserCfg );

  /* Disable interrupts
   */
  key = HwiP_disable();

  // Have to turn on the TRNG power before HalTRNG_InitTRNG
  // but must not repeat it every time the device wakes up
  // hence the call cannot be added to HalTRNG_InitTRNG();
#if !defined( USE_FPGA )
  Power_setDependency(PowerCC26XX_PERIPH_TRNG);
#endif

    // Setup the PRNG
    status = Random_seedAutomatic();

    if (status != Random_STATUS_SUCCESS) {
        System_abort("Random_seedAutomatic() failed");
    }

#ifdef FEATURE_MAC_SECURITY
#if !defined(DeviceFamily_CC13X2) && !defined(DeviceFamily_CC26X2)
  extern CryptoCC26XX_Handle Crypto_handle;

  CryptoCC26XX_init();
  CryptoCC26XX_Params_init(&CryptoCC26XXParams);
  Crypto_handle = CryptoCC26XX_open(Board_CRYPTO0, false, &CryptoCC26XXParams);
  if (!Crypto_handle)
  {
    /* abort */
      System_abort("Crypto open failed");
  }

  HalAesInit();
#else
#if !defined(USE_DMM) || !defined(BLE_START)
  AESCCM_init();
  AESCCM_Params_init(&AESCCMParams);
  AESCCMParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_POLLING;
  AESCCM_handle = AESCCM_open(Board_AESCCM0, &AESCCMParams);
#else
  //BLE Stack has opened the AESCCM driver
  extern AESCCM_Handle encHandleCCM;
  AESCCM_handle = encHandleCCM;
#endif /*!USE_DMM */
  if (!AESCCM_handle)
  {
      /* abort */
        System_abort("Crypto open failed");
  }
#endif /*!defined(DeviceFamily_CC13X2) && !defined(DeviceFamily_CC26X2)*/
#endif /* FEATURE_MAC_SECURITY */

  /* Initialize MAC */
  MAC_Init();

#ifdef COPROCESSOR
  /* Initialize the MAC function tables and features */
  MAC_InitDevice();
#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitCoord();
#endif /* FEATURE_FULL_FUNCTION_DEVICE */
#ifdef FEATURE_BEACON_MODE
  MAC_InitBeaconDevice();
#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitBeaconCoord();
#endif
#endif /* FEATURE_BEACON_MODE */
#else
  /* Initialize the MAC function tables and features */

#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitCoord();
#else /* FEATURE_FULL_FUNCTION_DEVICE */
  MAC_InitDevice();
#endif

#ifdef FEATURE_BEACON_MODE
#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitBeaconCoord();
#else
  MAC_InitBeaconDevice();
#endif /* FEATURE_FULL_FUNCTION_DEVICE */
#endif /* FEATURE_BEACON_MODE */
#endif /* COPROCESSOR */

#ifdef FEATURE_FREQ_HOP_MODE
  MAC_InitFH();
#endif /* FEATURE_FREQ_HOP_MODE */

   /* Initialize MAC buffer */
  macLowLevelBufferInit();

  /* Enable interrupts
   */
  HwiP_restore(key);

  // Must be done last
  macLowLevelInit();
}


/**************************************************************************************************
 * @fn          macTaskFxn
 *
 * @brief       This function is the main event handling function of the MAC executing
 *              in task context.  This function is called by OSAL when an event or message
 *              is pending for the MAC.
 *
 * input parameters
 *
 * @param       a0  Pointer to the MAC User Config structure
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
#include "dbg.h"
static void macTaskFxn(UArg a0, UArg a1)
{
  macEvent_t          *pMsg;
  macEventHdr_t       hdr;
  uint32_t            key;
  uint8               status;
//  uint32_t            macEvents = 0;

  macUserCfg_t* userCfg = (macUserCfg_t*) a0;

  /* initialize the MAC
   */
  macInit(userCfg);

  OsalPortTimers_registerCleanupEvent(_macTaskId, OSALPORT_CLEAN_UP_TIMERS_EVT);

  while(1)
  {
      macEvents = OsalPort_waitEvent(_macTaskId);

      Task_disable();

      OsalPort_clearEvent(_macTaskId, macEvents);
      DBG_PRINT1(0, "macEvents:%d", macEvents);

      /* handle mac symbol timer events */
      macSymbolTimerEventHandler();

      /* handle events on rx queue */
      if (macEvents & MAC_RX_QUEUE_TASK_EVT)
      {
        while ((pMsg = (macEvent_t *) OsalPort_msgDequeue(&macData.rxQueue)) != NULL)
        {
          /* Enter critical section
           */
          key = HwiP_disable();

          macData.rxCount--;

          /* Exit critical section
           */
          HwiP_restore(key);

          hdr.event = pMsg->hdr.event;

          macMain.pBuf = (uint8 *)pMsg;

          /* Check security processing result from the LMAC */
          status = pMsg->hdr.status;

          if (status == MAC_SUCCESS)
          {
            macExecute(pMsg);
          }
    #ifdef FEATURE_MAC_SECURITY
          else if ((status  == MAC_IMPROPER_KEY_TYPE) ||
                   (status  == MAC_IMPROPER_SECURITY_LEVEL) ||
                   (status  == MAC_SECURITY_ERROR) ||
                   (status  == MAC_UNAVAILABLE_KEY) ||
                   (status  == MAC_UNSUPPORTED_LEGACY) ||
                   (status  == MAC_UNSUPPORTED_SECURITY) ||
                   (status  == MAC_INVALID_PARAMETER) ||
                   (status  == MAC_COUNTER_ERROR))
          {
            macCommStatusInd(pMsg);

            /* discard invalid DATA_IND frame */
            if (hdr.event == MAC_RX_DATA_IND_EVT)
            {
              mac_msg_deallocate((uint8 **)&pMsg);
            }
          }
    #else
          else
          {
            /* discard invalid DATA_IND frame */
            if (hdr.event == MAC_RX_DATA_IND_EVT)
            {
              mac_msg_deallocate((uint8 **)&pMsg);
            }
          }
    #endif /* FEATURE_MAC_SECURITY */

          /* discard all frames except DATA_IND.
           * do not free data frames for app
           */
          if (hdr.event != MAC_RX_DATA_IND_EVT)
          {
              mac_msg_deallocate(&macMain.pBuf);
          }
        }
      }

      /* handle events on osal msg queue */
      if (macEvents & OsalPort_SYS_EVENT_MSG)
      {
        while ((pMsg = (macEvent_t *) OsalPort_msgReceive(_macTaskId)) != NULL)
        {
          if(pMsg->hdr.event >= 0xD0)
          {
            /* 0xE0 to 0xFF is reserved for App to send messages to TIMAC */
            macApp((macCmd_t *)pMsg);
          }
          else
          {
            macMain.pBuf = (uint8 *)pMsg;
            /* execute state machine */
            macExecute(pMsg);
            mac_msg_deallocate(&macMain.pBuf);
          }
        }
      }


      /* handle tx complete */
      if (macEvents & MAC_TX_COMPLETE_TASK_EVT)
      {
        hdr.event = MAC_INT_TX_COMPLETE_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle response wait timer */
      if (macEvents & MAC_RESPONSE_WAIT_TASK_EVT)
      {
        hdr.event = MAC_TIM_RESPONSE_WAIT_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle frame response timer */
      if (macEvents & MAC_FRAME_RESPONSE_TASK_EVT)
      {
        hdr.event = MAC_TIM_FRAME_RESPONSE_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle scan timer */
      if (macEvents & MAC_SCAN_TASK_EVT)
      {
        hdr.event = MAC_TIM_SCAN_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle indirect timer */
      if (macEvents & MAC_EXP_INDIRECT_TASK_EVT)
      {
        hdr.event = MAC_TIM_EXP_INDIRECT_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle start complete */
      if (macEvents & MAC_START_COMPLETE_TASK_EVT)
      {
        hdr.status = MAC_SUCCESS;
        hdr.event = MAC_INT_START_COMPLETE_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      /* handle pending broadcast */
      if (macEvents & MAC_BROADCAST_PEND_TASK_EVT)
      {
        hdr.status = MAC_SUCCESS;
        hdr.event = MAC_INT_BROADCAST_PEND_EVT;
        macExecute((macEvent_t *) &hdr);
      }

      if (macEvents & MAC_CSMA_TIM_TASK_EVT)
      {
        hdr.status = MAC_SUCCESS;
        hdr.event = MAC_CSMA_TIM_EXP_EVT ;
        macExecute((macEvent_t *) &hdr);
      }
      if (macEvents & MAC_TX_DATA_REQ_TASK_EVT )
      {
        hdr.status = MAC_SUCCESS;
        hdr.event = MAC_TX_DATA_REQ_PEND_EVT ;
        macExecute((macEvent_t *) &hdr);
      }
      if (macEvents & MAC_BC_TIM_TASK_EVT)
      {
        hdr.status = MAC_SUCCESS;
        hdr.event = MAC_BC_TIM_EXP_EVT ;
        macExecute((macEvent_t *) &hdr);
      }

      if (macEvents & OSALPORT_CLEAN_UP_TIMERS_EVT)
      {
        OsalPortTimers_cleanUpTimers();
      }

      /* handle pending message, if any */
      if (macMain.pPending != NULL)
      {
        uint32_t key;

        /* Disable interrupts
         */
        key = HwiP_disable();

        pMsg = macMain.pPending;
        macMain.pPending = NULL;

        /* Enable interrupts
         */
        HwiP_restore(key);

        macMain.pBuf = (uint8 *)pMsg;

        /* execute state machine */
        macExecute(pMsg);

        mac_msg_deallocate(&macMain.pBuf);
      }

      macEvents = 0;

      Task_enable();
  }
}

/**************************************************************************************************
 * @fn          macApp
 *
 * @brief       This function is called when Application sends MAC Stack a message.
 *
 * input parameters
 *
 * @param       pMsg - pointer to macCmd_t structure
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void macApp(macCmd_t *pMsg)
{
  uint8 dealloc = TRUE;
  /*
   * Set this flag to TRUE to send the message back to the app at the end
   * of the function.
   */
  uint8 sendMsg = FALSE;

  switch (pMsg->hdr.event)
  {
  case MAC_STACK_INIT_PARAMS:
   AppTaskId = pMsg->macInit.srctaskid;
   queryRetransmit = pMsg->macInit.retransmit;
   checkPending = pMsg->macInit.pendingMsg;
   pMacCbackQueryRetransmit = pMsg->macInit.pMacCbackQueryRetransmit;
   pMacCbackCheckPending = pMsg->macInit.pMacCbackCheckPending;
   dealloc = TRUE;
   break;
  }

  if(sendMsg == TRUE)
  {
    /* send message to App */
    OsalPort_msgSend(AppTaskId, (uint8 *)pMsg);
  }

  if(dealloc)
  {
    OsalPort_msgDeallocate((uint8 *)pMsg);
  }
}

/**************************************************************************************************
 *
 * @fn          MAC_CbackEvent
 *
 * @brief       This callback function sends MAC events to the application.
 *              The application must implement this function.  A typical
 *              implementation of this function would allocate an OSAL message,
 *              copy the event parameters to the message, and send the message
 *              to the application's OSAL event handler.  This function may be
 *              executed from task or interrupt context and therefore must
 *              be reentrant.
 *
 * @param       pData - Pointer to parameters structure.
 *
 * @return      None.
 *
 **************************************************************************************************
*/
void MAC_CbackEvent(macCbackEvent_t *pData)
{
  macCbackEvent_t *pMsg = NULL;

  uint8 len = msacbackSizeTable[pData->hdr.event];

  switch (pData->hdr.event)
  {
    case MAC_MLME_BEACON_NOTIFY_IND:
      if ( MAC_BEACON_STANDARD == pData->beaconNotifyInd.beaconType )
      {
        len += sizeof(macPanDesc_t) + pData->beaconNotifyInd.info.beaconData.sduLength +
             MAC_PEND_FIELDS_LEN(pData->beaconNotifyInd.info.beaconData.pendAddrSpec);
        if ((pMsg = (macCbackEvent_t *) OsalPort_msgAllocate(len)) != NULL)
        {
          /* Copy data over and pass them up */
          memcpy(pMsg, pData, sizeof(macMlmeBeaconNotifyInd_t));
          pMsg->beaconNotifyInd.info.beaconData.pPanDesc = (macPanDesc_t *) ((uint8 *) pMsg + sizeof(macMlmeBeaconNotifyInd_t));
          memcpy(pMsg->beaconNotifyInd.info.beaconData.pPanDesc, pData->beaconNotifyInd.info.beaconData.pPanDesc, sizeof(macPanDesc_t));
          pMsg->beaconNotifyInd.info.beaconData.pSdu = (uint8 *) (pMsg->beaconNotifyInd.info.beaconData.pPanDesc + 1);
          memcpy(pMsg->beaconNotifyInd.info.beaconData.pSdu, pData->beaconNotifyInd.info.beaconData.pSdu, pData->beaconNotifyInd.info.beaconData.sduLength);
        }
      }
      else if ( MAC_BEACON_ENHANCED == pData->beaconNotifyInd.beaconType )
      {
        len += sizeof(macPanDesc_t);

        if ((pMsg = (macCbackEvent_t *) OsalPort_msgAllocate(len)) != NULL)
        {
          /* Copy data over and pass them up */
          memcpy(pMsg, pData, sizeof(macMlmeBeaconNotifyInd_t));
          pMsg->beaconNotifyInd.info.eBeaconData.pPanDesc = (macPanDesc_t *) ((uint8 *) pMsg + sizeof(macMlmeBeaconNotifyInd_t));
          memcpy(pMsg->beaconNotifyInd.info.eBeaconData.pPanDesc, pData->beaconNotifyInd.info.eBeaconData.pPanDesc, sizeof(macPanDesc_t));
        }
      }
      break;

    case MAC_MCPS_DATA_IND:
    case MAC_MLME_WS_ASYNC_IND:
      pMsg = pData;
      break;


    default:
      pMsg = (macCbackEvent_t *)OsalPort_msgAllocate(len);
      if ((pMsg == NULL) && (pData->hdr.event == MAC_MCPS_DATA_CNF)
                      && (pData->dataCnf.pDataReq != NULL))
      {
        /*
         * The allocation failed for the data confirm, so we will
         * try to deallocate the memory used by the data request.
         * [OsalPort_msgDeallocate() will NULL pDataReq]
         */
          mac_msg_deallocate((uint8**)&(pData->dataCnf.pDataReq));

        /* Then, try the allocation again */
        pMsg = (macCbackEvent_t *) OsalPort_msgAllocate(len);
      }

      if (pMsg != NULL)
      {
        memcpy(pMsg, pData, len);
      }
      break;
  }

  if (pMsg != NULL)
  {
    OsalPort_msgSend(AppTaskId, (uint8 *) pMsg);
  }
}

/**************************************************************************************************
 * @fn          MAC_CbackQueryRetransmit
 *
 * @brief       This function callback function returns whether or not to continue MAC
 *              retransmission.
 *              A return value '0x00' will indicate no continuation of retry and a return value
 *              '0x01' will indicate to continue retransmission. This callback function shall be
 *              used to stop continuing retransmission for RF4CE.
 *              MAC shall call this callback function whenever it finishes transmitting a packet
 *              for macMaxFrameRetries times.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      0x00 to stop retransmission, 0x01 to continue retransmission.
 **************************************************************************************************
*/
uint8 MAC_CbackQueryRetransmit(void)
{
  uint8 reTransmit = queryRetransmit;

  if ( pMacCbackQueryRetransmit )
  {
    reTransmit = (*pMacCbackQueryRetransmit)();
  }

  return reTransmit;
}

/**************************************************************************************************
 * @fn          MAC_CbackCheckPending
 *
 * @brief       This callback function returns the number of pending indirect messages queued in
 *              the application. Most applications do not queue indirect data and can simply
 *              always return zero. The number of pending indirect messages only needs to be
 *              returned if macCfg.appPendingQueue to TRUE.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      The number of indirect messages queued in the application or zero.
 **************************************************************************************************
*/
uint8 MAC_CbackCheckPending(void)
{
  uint8 pend = checkPending;

  if ( pMacCbackCheckPending )
  {
    pend = (*pMacCbackCheckPending)();
  }

  return pend;
}



