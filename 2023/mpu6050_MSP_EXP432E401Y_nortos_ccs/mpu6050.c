/*
 * mpu6050.c
 *
 *  Created on: Oct 1, 2023
 *      Author: venki
 */
/* Standard Includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "uartstdio.h"

#define I2C_NUM_DATA    5
/* Defines for I2C bus parameters */
#define SLAVE_ADDRESS   0x68
uint8_t sendData[I2C_NUM_DATA] = {0x6B, 0x00, 0x75, 0x3B, 0x3C};
uint8_t dataIndex;
uint32_t systemClock;
void ConfigureUART(uint32_t systemClock);
uint8_t RAWData[6];

void I2C0Read(uint8_t slave_addr, uint8_t reg, uint8_t *data)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
    SysCtlDelay(500);
    I2CMasterDataPut(I2C0_BASE, reg);
    SysCtlDelay(500);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
    SysCtlDelay(500);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
    *data = I2CMasterDataGet(I2C0_BASE);
    SysCtlDelay(500);

}

void I2C0Write(uint8_t slave_addr, uint8_t reg, uint8_t data)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
    SysCtlDelay(500);
    I2CMasterDataPut(I2C0_BASE, reg);
    SysCtlDelay(500);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterDataPut(I2C0_BASE, data);
    SysCtlDelay(500);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
}


void MPU_get_whom_am_I()
{
    uint8_t whomAmI;
    while(I2CMasterBusy(I2C0_BASE)){};
    /* Put the Slave Address on the bus for Write */
    MAP_I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);
    SysCtlDelay(50);
    /* Write the first data to the bus */
    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);
    SysCtlDelay(50);
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE)){};

    /* Write the first data to the bus */
    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);
    SysCtlDelay(50);
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE)){};

    //Takes in the address of the device and the register to read from
    MAP_I2CMasterSlaveAddrSet (I2C0_BASE, SLAVE_ADDRESS, false); //find the device based on the address given
    SysCtlDelay(50);
    MAP_I2CMasterDataPut(I2C0_BASE, sendData[dataIndex++]);  //send the register to be read on to the I2C bus
    SysCtlDelay(50);
   // I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);   //send the send signal to send the register value
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);   //send the send signal to send the register value
    while (I2CMasterBusy(I2C0_BASE)){};
    SysCtlDelay(50);
    MAP_I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);  //set the master to read from the device
    SysCtlDelay(50);
    MAP_I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);    //send the receive signal to the device
    while (I2CMasterBusy(I2C0_BASE)){};
    SysCtlDelay(50);
    whomAmI = MAP_I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE)){};
    if (whomAmI != 0x68) {
        UARTprintf("Device Not Found\n");
        while(1);
    }
    else
        UARTprintf("Device Found\n");
}

void Init_MPU()
{
    UARTprintf("I2C Initialized\n");
    SysCtlDelay(50);
    MPU_get_whom_am_I();
}

char buf[100];
char* ftoa(float f)
{
    int pos=0,ix,dp,num;
    if (f<0)
    {
        buf[pos++]='-';
        f = -f;
    }
    dp=0;
    while (f>=10.0)
    {
        f=f/10.0;
        dp++;
    }
    for (ix=1;ix<8;ix++)
    {
        num = (int)f;
        f=f-num;
        if (num>9)
        buf[pos++]='#';
        else
        buf[pos++]='0'+num;
        if (dp==0) buf[pos++]='.';
        f=f*10.0;
        dp--;
    }
    return buf;
}

int main(void)
{
    int16_t xAccRaw;
    float xAcc;
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
    MAP_I2CMasterInitExpClk(I2C0_BASE, systemClock, true);
   // MAP_I2CMasterIntEnableEx(I2C0_BASE, I2C_MASTER_INT_NACK |
   //                                     I2C_MASTER_INT_STOP |
   //                                     I2C_MASTER_INT_DATA);

    Init_MPU();
    while(1)
    {
        I2C0Read(SLAVE_ADDRESS, 0x3B, &RAWData[0]);
        I2C0Read(SLAVE_ADDRESS, 0x3C, &RAWData[1]);
        xAccRaw = (int16_t)(RAWData[0]<<8  | RAWData[1]);
        if(xAccRaw > -32768 &&  xAccRaw < 32768 )
        {
            xAcc = xAccRaw/16384.0;
            UARTprintf("Xa= %s \n", ftoa(xAcc));
        }

        SysCtlDelay(500);
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

