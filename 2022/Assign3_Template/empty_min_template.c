/*
 *  ======== empty_min.c ========
 *  Jose Guevara, DA3, CPE403
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>


/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h> //all real time compatible
#include <ti/drivers/UART.h>


/* Board Header file */
#include "Board.h" //configuration comes from here

//
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "driverlib/i2c.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/adc.c"
#include "driverlib/sysctl.c"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/i2c.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "utils/uartstdio.c"
#include "utils/uartstdio.h"
//
//definitions and global variables
#define TASKSTACKSIZE   512
#define PWM_FREQUENCY 55


uint32_t count = 0; //gets current count of timer interrupts
char check; //gets input from user
uint32_t ui32PinStatus = 0x00000000; //variable to store the pin status of GPIO PortF

//PWM variables
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust = 83;

Semaphore_Handle taskSem;
Task_Handle ledToggleTask;



Task_Struct task0Struct;

Char task0Stack[TASKSTACKSIZE]; //heartbeat


/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void heartBeatFxn(UArg arg0, UArg arg1)
{
// Fill heartbeat function
}

//ALL TASKS

//ADC function
void task1(void)
{
    while(1){
        Semaphore_pend(taskSem1, BIOS_WAIT_FOREVER); //wait for Semaphore from ISR

            //ADC performed, store ADC value
	}

}

//UART function
void task2(void)
{
    while(1){
        Semaphore_pend(taskSem2, BIOS_WAIT_FOREVER); //wait for Semaphore from ISR

            //UART performed, display stored ADC value
	}

}

//PWM Update function
void task3(void)
{
     while(1){
        Semaphore_pend(taskSem3, BIOS_WAIT_FOREVER); //wait for Semaphore from ISR

            //Update PWM DC, from the value captured in  
	}


}

void Timer_ISR(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);         // must clear timer flag FROM timer

    // If count = 5 post task1 semaphore
     Semaphore_post(taskSem1); //posts semaphore for task to use upon  interrupt
    // If count = 10 post task2 semaphore
     Semaphore_post(taskSem2); //posts semaphore for task to use upon  interrupt
    // If count = 15 post task3 semaphore
     Semaphore_post(taskSem3); //posts semaphore for task to use upon  interrupt

    count++; //increases count that will be used by task function to determine which task to use
    if(count == 15){
            count = 0;
    }
}

void SWITCH_ISR(void)
{
  // Place the current ADC value as the PWM duty cycle.
}

void hardware_init(void)
{
    uint32_t ui32Period;

    //Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); //set PWM



	/* 

	Write GPIO intialization - LED = Output & SWITCH = Input IRQ
	// ADD Tiva-C GPIO setup - enables port, sets pins 1 pins for output
    	//switch1 (PF4) input

	*/

    // Turn on the LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 4);

    //PWM set up

	/* 

	Write PWM intialization & enable PWM

	*/
	
    // Timer 2 setup code

	/* 

	Write Time 2 intialization & enable interrupt

	*/

    //UART initialization

	/* 

	Write UART intialization - use uartstdio

	*/

    //ADC initialization

	/* 

	Write ADC intialization & select suitable sequence

	*/


}




/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral(); //initialize your board and GPIO's active
    Board_initGPIO();
    //Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initWatchdog();
    // Board_initWiFi();

    hardware_init(); //initialize hardware and timer

    //heartbeat idle task thread
    
    
    //tells system to creat a task1
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000; //amount of arguments
    taskParams.stackSize = TASKSTACKSIZE; //stack size (512 bytes)
    //stack space used to separate tasks
    taskParams.stack = &task0Stack; //points to the stack space
    Task_construct(&task0Struct, (Task_FuncPtr)task1, &taskParams, NULL);

    //tells system to creat a task2
    
    //tells system to creat a task3
        
    /* Start BIOS */
    BIOS_start(); //enable whole system (semaphore, kernel, scheduler, etc.)

    return (0);
}
