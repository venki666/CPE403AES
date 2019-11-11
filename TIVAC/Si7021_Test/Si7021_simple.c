/*
 * Si7021_simple.c
 *
 *  Created on: Nov 10, 2019
 *      Author: VenkatesanMuthukumar
 */



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"

#define SLAVE_ADDRESS           0x40
#define AMBIENT_TEMPERATURE     0xF3


void ConfigureUART(void);
void I2C_Init(void);
void I2C_Send(void);
uint16_t I2C_readMode(void);
uint16_t I2C_TempRead(void);
void Ambient_Temp(uint8_t ambient_temp_reg);


int main(void)
{
    uint16_t TemperatureR, TemperatureF;
    FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable the GPIO Port that is used for the on-board LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the GPIO Pins for the LEDs (PF1, PF2, PF3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Initialize the UART
    ConfigureUART();
    SysCtlDelay(1000000);
    UARTprintf("Program Starting....\n\n");
    SysCtlDelay(5000000);
    UARTprintf("UART Initialized\n");
    SysCtlDelay(5000000);

    I2C_Init();
    SysCtlDelay(5000000);

    while(1)
    {
      // I2C Send data to Slave Address - Ambient Temperature register - See MCP9808 datasheet
      I2C_Send();
      Ambient_Temp(AMBIENT_TEMPERATURE);
      while(I2CMasterBusy(I2C0_BASE))
      {
      }
      TemperatureR = I2C_TempRead();

      TemperatureF = ((175.25 * TemperatureR) / 65536.0) - 46.85;

      SysCtlDelay(SysCtlClockGet()/10/3);
      UARTprintf("Received Temperature data from Si7021: \n\r");
      UARTprintf("Si7021 I2C Sensor temperature readings: %u *C\n\r", TemperatureF);
      SysCtlDelay(5000000);

      //////////////////////////////////////////////////////////////////////
      // if (Temperature == 0x0400)     // Device ID
      // {  // Turn on Red LED
      //    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
      // }
      // else
      // {  // Turn on Green LED
      //    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
      // }
      //////////////////////////////////////////////////////////////////////

    }
}

void ConfigureUART(void)
{
  // Enable the GPIO Peripheral used by the UART
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

  // Configure the GPIO Pins for UART mode
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Use the internal 16MHz oscillator as the UART Clock source
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 9600, 16000000);
}

void I2C_Init()
{
    // Enable I2C1 peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
  SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

  // Enable GPIO Port B to be used for I2C0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);

  // Configure the pin muxing for I2C1 functions on Port B2 and B3
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

  I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
  // Set write mode
  UARTprintf("I2C Master init communication with Slave Address\n");
  SysCtlDelay(10000);
  UARTprintf("I2C Init complete!\n");
  SysCtlDelay(5000000);
}

void I2C_Send()
{
  // Specify Slave device address to write to
  I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);
  UARTprintf("Master transmit to Slave address\n\n");
}


void Ambient_Temp(uint8_t ambient_temp_reg)
{
  // Send Register address on Slave device
  I2CMasterDataPut(I2C0_BASE, ambient_temp_reg);
  UARTprintf("Writing to ambient temperature register on Slave address\n");

  // Initiate send of register address from Master to Slave
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
  UARTprintf("Ambient temperature register sent to Slave address\n");
}



uint16_t I2C_readMode()
{
  uint8_t UpperByte = 0;
  uint8_t LowerByte = 0;
  uint16_t data = 0;
  // Set read mode
  I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);

  // Get first byte from slave and ack for more
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
  while(I2CMasterBusy(I2C0_BASE));

  UpperByte = I2CMasterDataGet(I2C0_BASE);

  // Get second byte from slave and nack for complete
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
  while(I2CMasterBusy(I2C0_BASE));
  LowerByte = I2CMasterDataGet(I2C0_BASE);

  // See MCP9808 Data Sheet for each of the register information requested for
  data = UpperByte<<8|LowerByte;
  return data;
}

uint16_t I2C_TempRead()
{
  uint8_t UpperByte = 0;
  uint8_t LowerByte = 0;
  uint16_t Temperature = 0;
  // Set read mode
  I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);

  // Get first byte from slave and ack for more
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
  while(I2CMasterBusy(I2C0_BASE));
  UpperByte = I2CMasterDataGet(I2C0_BASE);

  // Get second byte from slave and nack for complete
  I2CMasterControl(I2C0_BASE,  I2C_MASTER_CMD_BURST_SEND_CONT);

  while(I2CMasterBusy(I2C0_BASE));
  LowerByte = I2CMasterDataGet(I2C0_BASE);

  Temperature = (UpperByte<<8) + LowerByte;
  return Temperature;
}




