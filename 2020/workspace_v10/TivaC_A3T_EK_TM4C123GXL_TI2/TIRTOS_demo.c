/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>				//needed for any Log_info() call
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles
#include <xdc/runtime/Diags.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Board Header file */
#include "Board.h"

/* Include header files for adc and GPIO functions */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>
#include <inc/hw_gpio.h>
#include "driverlib/uart.h"

volatile int16_t i16ToggleCount1 = 0;
volatile int16_t i16ToggleCount2 = 0;

//---------------------------------------------------------------------------
void delay_simple(void)
{
     SysCtlDelay(6700000);      // creates ~500ms delay - TivaWare fxn

}



void taskFxn1(void)
{
    while(1)
    {
    Semaphore_pend(task1sem, BIOS_WAIT_FOREVER);
    // LED values - 2=RED, 4=BLUE, 8=GREEN
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }

  //  delay_simple();                                // create a delay of ~1/2sec

    i16ToggleCount1 += 1;                    // keep track of #toggles

    Log_info1("LED2 TOGGLED [%u] times", i16ToggleCount1);    // send #toggles to Log Display
    //System_printf("Count: %d\n", i16ToggleCount);
    //System_flush();
    }
}


void taskFxn2(void)
{
    while(1)
    {
    Semaphore_pend(task2sem, BIOS_WAIT_FOREVER);
    // LED values - 2=RED, 4=BLUE, 8=GREEN
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
    }

  //  delay_simple();                                // create a delay of ~1/2sec

    i16ToggleCount2 += 1;                    // keep track of #toggles

    Log_info1("LED3 TOGGLED [%u] times", i16ToggleCount2);    // send #toggles to Log Display
    //System_printf("Count: %d\n", i16ToggleCount);
    //System_flush();
    }
}



//Timer 2 setup
void timer2Init()
{
    uint32_t ui32Period;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);			// enable Timer 2 periph clks
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);		// cfg Timer 2 mode - periodic

    ui32Period = (SysCtlClockGet()/500) / 2;
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period-1);			// set Timer 2 period

    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);		// enables Timer 2 to interrupt CPU

    TimerEnable(TIMER2_BASE, TIMER_A);						// enable Timer 2

}


volatile uint32_t tickCount=0;

/*
 *  ======== main ========
 */
int main()
{

    /* Set up the System Clock */
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    /* Enable all the peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);


    /* Unlock pin PF0 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    /* Configure Enable pin as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    timer2Init();
    BIOS_start();    /* Does not return */
}





//---------------------------------------------------------------------------
// Timer ISR  to be called by BIOS Hwi
//
// Posts Semaphore for releasing tasks
//---------------------------------------------------------------------------
void Timer_ISR(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);			// must clear timer flag FROM timer
    tickCount++;  //tickCount is incremented every 2 ms.

    if(tickCount == 300)
    {
        Semaphore_post(task1sem);
    }
    else if(tickCount == 600)
    {
        Semaphore_post(task2sem);
        tickCount = 0;
    }
}



