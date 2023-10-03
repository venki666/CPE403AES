/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432E4 Example project for ADC with single channel and single sequencer
 *
 * Description: In this application example the ADC0 is configured for a single
 * sequencer sampling a single channel in single ended mode. The data is then
 * displayed on the serial console.
 *
 *                MSP432E401Y
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST            PE3|<-- AIN0
 *            |                  |
 *            |                  |
 *            |                  |
 *            |               PA0|<--U0RX
 *            |               PA1|-->U0TX
 * Author: Amit Ashara
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Display Include via console */
#include "uartstdio.h"
uint32_t getADCValue[4];
volatile bool bgetConvStatus = false;;

void GPIOJ_IRQHandler(void)
{
    uint32_t getIntStatus;

    /* Get the interrupt status from the GPIO and clear the status */
    getIntStatus = MAP_GPIOIntStatus(GPIO_PORTJ_BASE, true);

    if((getIntStatus & GPIO_PIN_0) == GPIO_PIN_0)
    {
        MAP_GPIOIntClear(GPIO_PORTJ_BASE, getIntStatus);
        /* Wait for the conversion to complete */
        while(!bgetConvStatus);
        bgetConvStatus = false;

        /* Display the AIN0 (PE3) digital value on the console. */
        UARTprintf("AIN0 = %4d\n", (getADCValue[0]+getADCValue[1]+getADCValue[2]+getADCValue[3])/4);

    }
}

void ADC0SS2_IRQHandler(void)
{
    uint32_t getIntStatus;

    /* Get the interrupt status from the ADC */
    getIntStatus = MAP_ADCIntStatus(ADC0_BASE, 2, true);

    /* If the interrupt status for Sequencer-2 is set the
     * clear the status and read the data */
    if(getIntStatus == 0x4)
    {
        /* Clear the ADC interrupt flag. */
        MAP_ADCIntClear(ADC0_BASE, 2);

        /* Read ADC Value. */
        MAP_ADCSequenceDataGet(ADC0_BASE, 2, getADCValue);

        bgetConvStatus = true;
    }
}

void ConfigureUART(uint32_t systemClock)
{
    /* Enable the clock to GPIO port A and UART 0 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Configure the GPIO Port A for UART 0 */
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure the UART for 115200 bps 8-N-1 format */
    UARTStdioConfig(0, 115200, systemClock);
}

int main(void)
{
    uint32_t systemClock;

    /* Configure the system clock for 120 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);

    /* Initialize serial console */
    ConfigureUART(systemClock);

    /* Enable the clock to GPIO Port E and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)))
    {
    }

    /* Configure PE3 as ADC input channel */
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    /* Enable the clock to ADC-0 and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)))
    {
    }

    /* Enable the clock to the GPIO Port J and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)))
    {
    }

    /* Configure the GPIO PJ0 as input with internal pull up enabled. Configure
     * the PJ0 for a falling edge interrupt detection */
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //GPIOJ->PUR |= GPIO_PIN_0;
    MAP_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    MAP_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);

    MAP_IntEnable(INT_GPIOJ);

    /* Configure HW Averaging of 32x */
    MAP_ADCHardwareOversampleConfigure(ADC0_BASE, 32);

    /* Configure Sequencer 2 to sample a single analog channel : AIN0 */
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH0);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH0);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH0 | ADC_CTL_IE |
                                 ADC_CTL_END);


    /* Enable sample sequence 2 with a processor signal trigger.  Sequence 3
     * will do a single sample when the processor sends a signal to start the
     * conversion */
    MAP_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_TIMER, 0);

    /* Since sample sequence 2 is now configured, it must be enabled. */
    MAP_ADCSequenceEnable(ADC0_BASE, 2);

    /* Clear the interrupt status flag.  This is done to make sure the
     * interrupt flag is cleared before we sample. */
    MAP_ADCIntClear(ADC0_BASE, 2);
    MAP_ADCIntEnable(ADC0_BASE, 2);
    /* Enable the Interrupt generation from the ADC-0 Sequencer */
    MAP_IntEnable(INT_ADC0SS2);
    /* Enable Timer-0 clock and configure the timer in periodic mode with
     * a frequency of 10 KHz. Enable the ADC trigger generation from the
     * timer-0. */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)))
    {
    }

    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, (systemClock/10000));
    MAP_TimerADCEventSet(TIMER0_BASE, TIMER_ADC_TIMEOUT_A);
    MAP_TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
    MAP_TimerEnable(TIMER0_BASE, TIMER_A);


    /* Sample AIN0 forever.  Display the value on the console. */
    while(1)
    {


    }
}
