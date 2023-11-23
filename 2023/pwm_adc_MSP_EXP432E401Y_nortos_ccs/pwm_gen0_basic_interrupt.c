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
 * MSP432E4 PWM with basic interrupt
 *
 * Description: This project creates two PWMs from PWM Generator 3.  Each PWM
 *              period is 128K clock cycles(64K up and 64K down); ~250Hz.  PWMA
 *              has a 25% positive duty cycle while PWMB has 75%.  The PWM
 *              interrupt sources for Generator 0 are the PWMA comparator value
 *              in both the up and down directions and the Load and Zero count
 *              values.
 *
 *                MSP432E401Y
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |               PA0|<--- UART RX
 *            |               PA1|---> UART TX
 *            |                  |
 *            |               PE3|---< ADC0
 *            |               PF1|---> ADC  Duty Cycle
 *            |                  |
 *
 * Author: C. Sterzik
 *
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>



/* Global variable for system clock */
uint32_t getSystemClock;

/* PWM ISR */
/*
void PWM0_0_IRQHandler(void)
{
    uint32_t getIntStatus;

    getIntStatus = MAP_PWMGenIntStatus(PWM0_BASE, PWM_GEN_0, true);

    MAP_PWMGenIntClear(PWM0_BASE, PWM_GEN_0, getIntStatus);


}
*/

int main(void)
{
    uint32_t getADCValue[1];
    /* Configure the system clock for 16 MHz internal oscillator */
    getSystemClock = MAP_SysCtlClockFreqSet((SYSCTL_OSC_INT |
                                             SYSCTL_USE_OSC), 16000000);

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


    /* Configure Sequencer 3 to sample a single analog channel : AIN0 */
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                 ADC_CTL_END);

    /* Enable sample sequence 3 with a processor signal trigger.  Sequence 3
     * will do a single sample when the processor sends a signal to start the
     * conversion */
    MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    /* Since sample sequence 3 is now configured, it must be enabled. */
    MAP_ADCSequenceEnable(ADC0_BASE, 3);

    /* Clear the interrupt status flag.  This is done to make sure the
     * interrupt flag is cleared before we sample. */
    MAP_ADCIntClear(ADC0_BASE, 3);

    /* The PWM peripheral must be enabled for use. */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)));

    /* Set the PWM clock to the system clock. */
    MAP_PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);

    /* Enable the clock to the GPIO Port F for PWM pins */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    MAP_GPIOPinConfigure(GPIO_PF1_M0PWM1);
    MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    /* Configure the PWM0 to count up/down without synchronization. */
    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

    /* Set the PWM period to 250Hz.  To calculate the appropriate parameter
     * use the following equation: N = (1 / f) * SysClk.  Where N is the
     * function parameter, f is the desired frequency, and SysClk is the
     * system clock frequency.
     * In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
     * the maximum period you can set is 2^16 - 1. */
    MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 64000);

    /* Set PWM0 PF1 to a duty cycle of 75%.  You set the duty cycle as a
     * function of the period.  Since the period was set above, you can use the
     * PWMGenPeriodGet() function.  For this example the PWM will be high for
     * 7% of the time or 16000 clock cycles 3*(64000 / 4). */
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,
                     3*MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) / 2);

    //MAP_IntMasterEnable();

    /* This timer is in up-down mode.  Interrupts will occur when the
     * counter for this PWM counts to the load value (64000), when the
     * counter counts up to 64000/4 (PWM A Up), counts down to 64000/4
     * (PWM A Down), and counts to 0. */
    //MAP_PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_0,
    //                        PWM_INT_CNT_ZERO | PWM_INT_CNT_LOAD |
    //                        PWM_INT_CNT_AU | PWM_INT_CNT_AD);
   // MAP_IntEnable(INT_PWM0_0);
   // MAP_PWMIntEnable(PWM0_BASE, PWM_INT_GEN_0);

    /* Enable the PWM0 Bit 0 (PF0) and Bit 1 (PF1) output signals. */
    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);

    /* Enables the counter for a PWM generator block. */
    MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); // Enable
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); // !Sleep

    /* Loop forever while the PWM signals are generated. */
    while(1)
    {
        /* Trigger the ADC conversion. */
        MAP_ADCProcessorTrigger(ADC0_BASE, 3);

        /* Wait for conversion to be completed. */
        while(!MAP_ADCIntStatus(ADC0_BASE, 3, false))
        {
        }

        /* Clear the ADC interrupt flag. */
        MAP_ADCIntClear(ADC0_BASE, 3);

        /* Read ADC Value. */
        MAP_ADCSequenceDataGet(ADC0_BASE, 3, getADCValue);

        if(getADCValue[0] <= 2000)
        {
            // Create a condition for QEI reading
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x1);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x1);
            MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 4000);
        }
        else if (getADCValue[0] >= 2100)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x1);
            MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 4000);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
        }



        /* Delay the next sampling */
        MAP_SysCtlDelay(getSystemClock / 12);
    }
}