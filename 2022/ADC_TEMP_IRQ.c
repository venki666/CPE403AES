/*
 *	adci.c
 *	adc with interrupt to measure temp
 *	$Id:$
 *
 *	NOTE! add ADC0handler to startup_gcc.c (or whatever your IDE uses)
 *		void ADC0handler(void);		// declare it
 *		...
 *		ADC0handler,			// ADC Sequence 3
 *	
 *	LED indicators
 *	  red    PF1  triggered conversion, waiting for conversion to finish
 *	  blue   PF2  inside ADC interuppt handler, reading conversion data
 *	  green  PF3  got conversion
 *
 *	To make things take longer, to maybe see better on a scope,
 *	both the processor and ADC run from from slower clocks,
 *	and the ADC uses hardware averaging.
 *
 *	The repetition rate of the loop is about 775Hz.
 *	It waits about 137 usec for a conversion complete.
 *	The ADC interrupt routine takes about 9.5 usec.
 *
 *	About every two seconds the loop prints a line to UART0
 *		$08dc 2268    10 C   50 F
 *
 *	Warm up (or cool off) your thumb, and hold against the chip,
 *	to see the temperature change.
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// define the peripheral/port/pins for the LEDs 
#define LED_PERIF SYSCTL_PERIPH_GPIOF
#define LED_PORT GPIO_PORTF_BASE
#define LED_RED GPIO_PIN_1
#define LED_GREEN GPIO_PIN_3
#define LED_BLUE GPIO_PIN_2
#define LEDS(LED_RED | LED_GREEN | LED_BLUE)
#define LED_SET(X) GPIOPinWrite(LED_PORT, LEDS, (X));

// this array must be big enough to hold the channels we convert
// (only need one entry for this example)
uint32_t rawtemp[4];

// this flag is set by the interrupt routine, and read by the wait loop
volatile uint32_t newtemp;

// this is the interrupt routine for the ADC
void ADC0handler(void)
{
	LED_SET(LED_BLUE);
	// clear the interrupt flag, grab the data, set the 'done' flag
	ADCIntClear(ADC0_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, rawtemp);
	newtemp = 1;
	LED_SET(0);
}

void InitADC(void)
{
	// enable ADC0
	// set a slower conversion rate
	// use sequencer 3 (single sample), with manual trigger
	// seq 3, step 0, temperature-sensor, interrupt-enable, last-conversion
	// enable sequencer 3

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlDelay(2);
	ADCClockConfigSet(ADC0_BASE,
		ADC_CLOCK_RATE_EIGHTH | ADC_CLOCK_SRC_PIOSC, 1);
	ADCHardwareOversampleConfigure(ADC0_BASE, 16);	// 64 32 16 8 4 2
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
		ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);

	// make sure interrupt flag is clear
	// enable the interrupt for the module and for the sequence
	ADCIntClear(ADC0_BASE, 3);
	IntEnable(INT_ADC0SS3);
	ADCIntEnable(ADC0_BASE, 3);
}

void InitConsole(void)
{
	// enable the port that has UART0 pins
	// configure the RX/TX port pins
	// enable the uart, set the clock, set the pins
	// set baud-rate
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlDelay(2);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioConfig(0, 115200, 16000000);
}

int main(void)
{
	uint32_t i, tempC, tempF;

	// clock at 8 MHz from the crystal
	SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(LED_PERIF);
	SysCtlDelay(2);
	GPIOPinTypeGPIOOutput(LED_PORT, LEDS);
	LED_SET(0);

	InitConsole();
	InitADC();

	UARTprintf("\n\nADC internal temperature sensor\n");

	// allow interrupts
	IntMasterEnable();

	i = 0;
	while (1)
	{
		// clear the flag, show we're waiting
		newtemp = 0;
		LED_SET(LED_RED);

		// start a conversion
		ADCProcessorTrigger(ADC0_BASE, 3);

		// spin and wait for the interrupt handler to get the data
		while (newtemp == 0)
		{
			__asm("WFE\n	");
		}

		LED_SET(LED_GREEN);

		// print temps about every two seconds
		if (++i == 2000)
		{
			i = 0;
			tempC = (1475 - ((2475 *rawtemp[0])) / 4096) / 10;
			tempF = ((tempC *9) + 160) / 5;
			UARTprintf("$%04x %4d   %3d C  %3d F\n",
				rawtemp[0], rawtemp[0], tempC, tempF);
		}
		// delay to slow down the loop to something around 1KHz
		SysCtlDelay(SysCtlClockGet() *3 / 8000);
	}
}