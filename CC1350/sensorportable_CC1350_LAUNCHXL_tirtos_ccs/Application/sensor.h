/******************************************************************************

 @file sensor.h

 @brief TIMAC 2.0 Sensor Example Application Header

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
#ifndef SENSOR_H
#define SENSOR_H

/******************************************************************************
 Includes
 *****************************************************************************/

#include "smsgs.h"

#ifdef OSAL_PORT2TIRTOS
#include <ti/sysbios/knl/Task.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/*! Event ID - Start the device in the network */
#define SENSOR_START_EVT 0x0001
/*! Event ID - Reading Timeout Event */
#define SENSOR_READING_TIMEOUT_EVT 0x0002
/*! Clock tick period  */
#define CLOCK_TICK_PERIOD     (10)
/*! tick number for one ms  */
#define TICKPERIOD_MS_US      (1000/(CLOCK_TICK_PERIOD))

#ifdef FEATURE_NATIVE_OAD
/*! Event ID - OAD Timeout Event */
#define SENSOR_OAD_TIMEOUT_EVT 0x0004
#ifdef OAD_IMG_A
#define SENSOR_OAD_SEND_RESET_RSP_EVT 0x0010
#endif
#endif //FEATURE_NATIVE_OAD

#ifdef DISPLAY_PER_STATS
/*! Event ID - Update Sensor Stats Event */
#define SENSOR_UPDATE_STATS_EVT 0x0008
#endif /* DISPLAY_PER_STATS */

/*! Sensor Status Values */
typedef enum
{
    /*! Success */
    Sensor_status_success = 0,
    /*! Sensor isn't in the correct state to send a message */
    Sensor_status_invalid_state = 1
} Sensor_status_t;

/******************************************************************************
 Structures
 *****************************************************************************/

/******************************************************************************
 Global Variables
 *****************************************************************************/

/*! Sensor Task ID */
extern uint8_t Sensor_TaskId;

/*! Sensor events flags */
extern uint16_t Sensor_events;

/*! Sensor statistics */
extern Smsgs_msgStatsField_t Sensor_msgStats;

#ifdef POWER_MEAS
/*! Power Measurement Statistics */
 extern Smsgs_powerMeastatsField_t Sensor_pwrMeasStats;
#endif

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/*!
 * @brief Initialize this application.
 *
 * @param       macTaskHndl - The MAC Task ID to send messages to.
 */
#ifdef OSAL_PORT2TIRTOS
extern void Sensor_init(uint8_t _macTaskId);
#else
extern void Sensor_init(void);
#endif

/*!
 * @brief Application task processing.
 */
extern void Sensor_process(void);

/*!
 * @brief   Send MAC data request
 *
 * @param   type - message type
 * @param   pDstAddr - destination address
 * @param   rxOnIdle - true if not a sleepy device
 * @param   len - length of payload
 * @param   pData - pointer to the buffer
 *
 * @return  true if sent, false if not
 */
extern bool Sensor_sendMsg(Smsgs_cmdIds_t type, ApiMac_sAddr_t *pDstAddr,
                    bool rxOnIdle, uint16_t len, uint8_t *pData);
#ifdef IDENTIFY_LED
/*!
 * @brief Send identify LED request to collector
 */
extern void Sensor_sendIdentifyLedRequest(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H */
