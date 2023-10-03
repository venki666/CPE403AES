/*
 * mpu6050.c
 *
 *  Created on: Oct 1, 2023
 *      Author: venki
 */
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "uartstdio.h"

#define I2C_NUM_DATA    4
/* Defines for I2C bus parameters */
#define SLAVE_ADDRESS   0x68
uint8_t sendData[I2C_NUM_DATA] = {0x6B, 0x00, 0x75, 0x3B};
uint8_t dataIndex;
uint32_t systemClock;
void ConfigureUART(uint32_t systemClock);

int main(void)
{

    uint32_t whomAmI;
    uint32_t xAccRaw;
    /* Configure the system clock for 120 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);
    ConfigureUART(systemClock);

    /* Enable clocks to GPIO Port B and configure pins as I2C */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)))
    {
    }

    MAP_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    MAP_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    MAP_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    MAP_GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);


    /* Enable the clock to I2C-0 module and configure the I2C Master */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0)))
    {
    }

    /* Configure the I2C Master in standard mode and enable interrupt for Data
     * completion, NAK and Stop condition on the bus */
    MAP_I2CMasterInitExpClk(I2C0_BASE, systemClock, false);
    MAP_I2CMasterIntEnableEx(I2C0_BASE, I2C_MASTER_INT_NACK |
                                        I2C_MASTER_INT_STOP |
                                        I2C_MASTER_INT_DATA);

    /* Put the Slave Address on the bus for Write */
    MAP_I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);
    while(I2CMasterBusy(I2C0_BASE)){};
    /* Write the first data to the bus */
    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE)){};

    /* Write the first data to the bus */
    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE)){};
    SysCtlDelay(50000);
    //Takes in the address of the device and the register to read from
    MAP_I2CMasterSlaveAddrSet (I2C0_BASE, SLAVE_ADDRESS, false); //find the device based on the address given
    while (I2CMasterBusy(I2C0_BASE)){};

    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);  //send the register to be read on to the I2C bus
    while (I2CMasterBusy(I2C0_BASE)){};

   // I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);   //send the send signal to send the register value
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);   //send the send signal to send the register value
    while (I2CMasterBusy(I2C0_BASE)){};
    SysCtlDelay(50000);
    MAP_I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);  //set the master to read from the device
    while (I2CMasterBusy(I2C0_BASE)){};

    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);    //send the receive signal to the device
    while (I2CMasterBusy(I2C0_BASE)){};
    SysCtlDelay(50000);
    whomAmI = MAP_I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE)){};
    if (whomAmI != 0x68) {
        UARTprintf("Device Not Found\n");
        while(1);
    }
    else
        UARTprintf("Device Found\n");
    while(1)
    {
        //Takes in the address of the device and the register to read from
        MAP_I2CMasterSlaveAddrSet (I2C0_BASE, SLAVE_ADDRESS, false); //find the device based on the address given

        MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);  //send the register to be read on to the I2C bus
        while (I2CMasterBusy(I2C0_BASE)){};

       // I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);   //send the send signal to send the register value
        MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);   //send the send signal to send the register value
        while (I2CMasterBusy(I2C0_BASE)){};
        MAP_I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);  //set the master to read from the device
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
        while(I2CMasterBusy(I2C0_BASE));
        xAccRaw = I2CMasterDataGet(I2C0_BASE)<<8;
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        while(I2CMasterBusy(I2C0_BASE));
        xAccRaw = xAccRaw | I2CMasterDataGet(I2C0_BASE);
        UARTprintf("X_a = %4d\n", xAccRaw);
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

