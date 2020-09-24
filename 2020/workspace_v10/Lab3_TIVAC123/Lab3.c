
/* -----------------------          Include Files       --------------------- */
#include <stdint.h>                     // Library of Standard Integer Types
#include <stdbool.h>                    // Library of Standard Boolean Types
#include "inc/hw_memmap.h"              // Macros defining the memory map of the Tiva C Series device
#include "inc/hw_gpio.h"                // Defines macros for GPIO hardware
#include "inc/hw_types.h"               // Defines common types and macros
#include "driverlib/sysctl.h"           // Defines and macros for System Control API of DriverLib
#include "driverlib/pin_map.h"          // Mapping of peripherals to pins of all parts
#include "driverlib/gpio.h"             // Defines and macros for GPIO API of DriverLib

/* -----------------------      Global Variables        --------------------- */
uint32_t ui32PinStatus = 0x00000000;    // Variable to store the pin status of GPIO PortF

/* -----------------------          Main Program        --------------------- */
int main(void){

    // Set the System clock to 80MHz and enable the clock for peripheral PortF.
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Set the PF1, PF2, PF3 as output and PF4 as input.
    // Connect PF4 to internal Pull-up resistors and set 2 mA as current strength.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Start an Infinite Loop
    while (true){
        // Read the status of Input
        ui32PinStatus = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

       if (!ui32PinStatus){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            SysCtlDelay(20000000);
        }else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
        }
    }
}
