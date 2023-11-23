/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== mutex.c ========
 */

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Hwi.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/Board.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* "useHwiMacros" allows for faster inline disable/enable */
#define ti_sysbios_Build_useHwiMacros
    #define HWI_INT 67
    #define CONFIGURE_INTERRUPTS()
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT);
    #define CLEAR_INTERRUPT()
    #define CHANGE_HWI(hwiHandle, ToggleLedHWI) \
        Hwi_setFunc(hwiHandle, ToggleLedHWI, (UArg)NULL);
#define TASKSTACKSIZE   512

Void task1Fxn(UArg arg0, UArg arg1);
Void task2Fxn(UArg arg0, UArg arg1);
void ToggleLedHWI(UArg arg);
Int resource = 0;
Int finishCount = 0;
UInt32 sleepTickCount;

Task_Struct task1Struct, task2Struct;
Char task1Stack[TASKSTACKSIZE], task2Stack[TASKSTACKSIZE];
Semaphore_Struct semStruct;
Semaphore_Handle semHandle;

/*
 *  ======== main ========
 */
int main()
{
    /* Construct BIOS objects */
    Task_Params taskParams;
    Semaphore_Params semParams;

    /* Call driver init functions */
    Board_init();
    GPIO_init();

    /* Construct writer/reader Task threads */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task1Stack;
    taskParams.priority = 1;
    Task_construct(&task1Struct, (Task_FuncPtr)task1Fxn, &taskParams, NULL);

    taskParams.stack = &task2Stack;
    taskParams.priority = 2;
    Task_construct(&task2Struct, (Task_FuncPtr)task2Fxn, &taskParams, NULL);

    /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semStruct, 1, &semParams);

    /* Obtain instance handle */
    semHandle = Semaphore_handle(&semStruct);

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);
    CONFIGURE_INTERRUPTS();
    /* We want to sleep for 10000 microseconds */
    sleepTickCount = 10000 / Clock_tickPeriod;

    BIOS_start();    /* Does not return */
    return(0);
}

/*
 *  ======== task1Fxn ========
 */
Void task1Fxn(UArg arg0, UArg arg1)
{

    for (;;) {
        if (Semaphore_getCount(semHandle) == 0) {
            System_printf("Sem blocked in task1\n");
        }

        /* Get access to resource */
        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

        GPIO_toggle(CONFIG_GPIO_LED_1);

        Semaphore_post(semHandle);

        Task_sleep(sleepTickCount);
    }
}

/*
 *  ======== task2Fxn ========
 */
Void task2Fxn(UArg arg0, UArg arg1)
{
    for (;;) {

        if (Semaphore_getCount(semHandle) == 0) {
            System_printf("Sem blocked in task2\n");
        }

        /* Get access to resource */
        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

        GPIO_toggle(CONFIG_GPIO_LED_2);

        Semaphore_post(semHandle);

        Task_sleep(sleepTickCount);

    }
}

void ToggleLedHWI(UArg arg){
    CLEAR_INTERRUPT();
    /* Toggle an LED */
    GPIO_toggle(CONFIG_GPIO_LED_0);
}
