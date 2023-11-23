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
 *  ======== hello.c ========
 */

#include <stdint.h>
#include <stddef.h>
/* For usleep() */
#include <unistd.h>
/* XDC Module Headers */


/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/GPIO.h>

#include <ti/drivers/Board.h>
/* Driver configuration */
#include "ti_drivers_config.h"
#define TASKSTACKSIZE           512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
Swi_Struct swi0Struct, swi1Struct;
Swi_Handle swi0Handle, swi1Handle;
Clock_Struct clk0Struct;
Semaphore_Struct sem0Struct;
Semaphore_Handle sem0Handle;

/*
 *  ======== swi0Fxn =======
 */
Void swi0Fxn(UArg arg0, UArg arg1)
{
   // GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    GPIO_toggle(CONFIG_GPIO_LED_0);
}

/*
 *  ======== swi1Fxn =======
 */
Void swi1Fxn(UArg arg0, UArg arg1)
{
    //GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    GPIO_toggle(CONFIG_GPIO_LED_1);
}

/*
 *  ======== clk0Fxn =======
 */
Void clk0Fxn(UArg arg0)
{
    Swi_post(swi0Handle);
    Swi_post(swi1Handle);

    Semaphore_post(sem0Handle);
}

/*
 *  ======== task0Fxn =======
 */
Void task0Fxn(UArg arg0, UArg arg1)
{
    while(1)
    {
    /* wait for swis to be posted from Clock function */
    Semaphore_pend(sem0Handle, BIOS_WAIT_FOREVER);
    GPIO_toggle(CONFIG_GPIO_LED_2);
    //GPIO_write(CONFIG_GPIO_LED_2, CONFIG_GPIO_LED_ON);
    }
}

/*
 *  ======== main ========
 */
int main()
{
    /* Construct BIOS objects */
    Task_Params taskParams;
    Swi_Params swiParams;
    Semaphore_Params semParams;
    Clock_Params clkParams;

    /* Call driver init functions */
    Board_init();
    /* Call driver init functions */
    GPIO_init();
    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);


    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 3;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)task0Fxn, &taskParams, NULL);

    Swi_Params_init(&swiParams);
    swiParams.arg0 = 1;
    swiParams.arg1 = 0;
    swiParams.priority = 2;
    swiParams.trigger = 0;

    Swi_construct(&swi0Struct, (Swi_FuncPtr)swi0Fxn, &swiParams, NULL);
    swi0Handle = Swi_handle(&swi0Struct);

    swiParams.arg0 = 2;
    swiParams.arg1 = 0;
    swiParams.priority = 1;
    swiParams.trigger = 3;

    Swi_construct(&swi1Struct, (Swi_FuncPtr)swi1Fxn, &swiParams, NULL);
    swi1Handle = Swi_handle(&swi1Struct);

    /* Construct a Semaphore object to be used as a resource lock, inital count 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&sem0Struct, 0, &semParams);

    /* Obtain instance handle */
    sem0Handle = Semaphore_handle(&sem0Struct);

    Clock_Params_init(&clkParams);
    clkParams.period = 500000/Clock_tickPeriod;
    clkParams.startFlag = TRUE;

    /* Construct a periodic Clock Instance with period 5000/Clock_tickPeriod system time units */
    Clock_construct(&clk0Struct, (Clock_FuncPtr)clk0Fxn,
                    500000/Clock_tickPeriod, &clkParams);

    BIOS_start();    /* Does not return */
    return(0);
}
