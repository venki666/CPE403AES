// HEADER FILES
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"driverlib/gpio.h"
#include"driverlib/pin_map.h"
#include"driverlib/sysctl.h"
#include"driverlib/uart.h"
#define GPIO_PA0_U0RX 0x00000001 // UART PIN ADDRESS FOR UART RX
#define GPIO_PA1_U0TX 0x00000401 // UART PIN ADDRESS FOR UART TX
int main(void)
{
    // SYSTEM CLOCK AT 40 MHZ
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|
    SYSCTL_XTAL_16MHZ);
    // ENABLE PERIPHERAL UART 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // ENABLE GPIO PORT A,FOR UART
    GPIOPinConfigure(GPIO_PA0_U0RX); // PA0 IS CONFIGURED TO UART RX
    GPIOPinConfigure(GPIO_PA1_U0TX); // PA1 IS CONFIGURED TO UART TX
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // CONFIGURE UART, BAUD RATE 115200, DATA BITS 8, STOP BIT 1, PARITY NONE
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 'c');
    UARTCharPut(UART0_BASE, 'h');
    UARTCharPut(UART0_BASE, 'o'); // SEND "Echo Output: " IN UART
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'O');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, 'p');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, '\n');
    while (1)
     {
        //UART ECHO - what is received is transmitted back //
        if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE,
        UARTCharGet(UART0_BASE));
     }
}
