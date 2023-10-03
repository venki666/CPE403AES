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
 * MSP432E4 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432E401Y
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "uartstdio.h"
#include <string.h>

// Device address when AD0 is grounded
#define MPU6050_ADDR (0x68)

// MPU6050 Definitions
#define XG_OFFS_TC          0x00
#define YG_OFFS_TC          0x01
#define ZG_OFFS_TC          0x02
#define X_FINE_GAIN         0x03
#define Y_FINE_GAIN         0x04
#define Z_FINE_GAIN         0x05
#define XA_OFFS_H           0x06
#define XA_OFFS_L_TC        0x07
#define YA_OFFS_H           0x08
#define YA_OFFS_L_TC        0x09
#define ZA_OFFS_H           0x0A
#define ZA_OFFS_L_TC        0x0B
#define XG_OFFS_USRH        0x13
#define XG_OFFS_USRL        0x14
#define YG_OFFS_USRH        0x15
#define YG_OFFS_USRL        0x16
#define ZG_OFFS_USRH        0x17
#define ZG_OFFS_USRL        0x18
#define SMPLRT_DIV          0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define FF_THR              0x1D
#define FF_DUR              0x1E
#define MOT_THR             0x1F
#define MOT_DUR             0x20
#define ZRMOT_THR           0x21
#define ZRMOT_DUR           0x22
#define FIFO_EN             0x23
#define I2C_MST_CTRL        0x24
#define I2C_SLV0_ADDR       0x25
#define I2C_SLV0_REG        0x26
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV1_ADDR       0x28
#define I2C_SLV1_REG        0x29
#define I2C_SLV1_CTRL       0x2A
#define I2C_SLV2_ADDR       0x2B
#define I2C_SLV2_REG        0x2C
#define I2C_SLV2_CTRL       0x2D
#define I2C_SLV3_ADDR       0x2E
#define I2C_SLV3_REG        0x2F
#define I2C_SLV3_CTRL       0x30
#define I2C_SLV4_ADDR       0x31
#define I2C_SLV4_REG        0x32
#define I2C_SLV4_DO         0x33
#define I2C_SLV4_CTRL       0x34
#define I2C_SLV4_DI         0x35
#define I2C_MST_STATUS      0x36
#define INT_PIN_CFG         0x37
#define INT_ENABLE          0x38
#define DMP_INT_STATUS      0x39
#define INT_STATUS          0x3A
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
#define EXT_SENS_DATA_00    0x49
#define EXT_SENS_DATA_01    0x4A
#define EXT_SENS_DATA_02    0x4B
#define EXT_SENS_DATA_03    0x4C
#define EXT_SENS_DATA_04    0x4D
#define EXT_SENS_DATA_05    0x4E
#define EXT_SENS_DATA_06    0x4F
#define EXT_SENS_DATA_07    0x50
#define EXT_SENS_DATA_08    0x51
#define EXT_SENS_DATA_09    0x52
#define EXT_SENS_DATA_10    0x53
#define EXT_SENS_DATA_11    0x54
#define EXT_SENS_DATA_12    0x55
#define EXT_SENS_DATA_13    0x56
#define EXT_SENS_DATA_14    0x57
#define EXT_SENS_DATA_15    0x58
#define EXT_SENS_DATA_16    0x59
#define EXT_SENS_DATA_17    0x5A
#define EXT_SENS_DATA_18    0x5B
#define EXT_SENS_DATA_19    0x5C
#define EXT_SENS_DATA_20    0x5D
#define EXT_SENS_DATA_21    0x5E
#define EXT_SENS_DATA_22    0x5F
#define EXT_SENS_DATA_23    0x60
#define MOT_DETECT_STATUS   0x61
#define I2C_SLV0_DO         0x63
#define I2C_SLV1_DO         0x64
#define I2C_SLV2_DO         0x65
#define I2C_SLV3_DO         0x66
#define I2C_MST_DELAY_CTRL  0x67
#define SIGNAL_PATH_RESET   0x68
#define MOT_DETECT_CTRL     0x69
#define USER_CTRL           0x6A
#define PWR_MGMT_1          0x6B
#define PWR_MGMT_2          0x6C
#define BANK_SEL            0x6D
#define MEM_START_ADDR      0x6E
#define MEM_R_W             0x6F
#define DMP_CFG_1           0x70
#define DMP_CFG_2           0x71
#define FIFO_COUNTH         0x72
#define FIFO_COUNTL         0x73
#define FIFO_R_W            0x74
#define WHO_AM_I            0x75

uint32_t systemClock;
void I2C0_init(void);
void MPU6050_init(void);
void MPU_get_whom_am_I(void);
void I2C0_Write(uint8_t, uint8_t, uint8_t);
uint32_t I2C0_Read (uint8_t, uint8_t);

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

 //Read data from slave to master
 uint32_t I2C0_Read (uint8_t addr, uint8_t reg) {
     //Takes in the address of the device and the register to read from
     I2CMasterSlaveAddrSet (I2C0_BASE, addr, false); //find the device based on the address given
     while (I2CMasterBusy (I2C0_BASE));

     I2CMasterDataPut (I2C0_BASE, reg);  //send the register to be read on to the I2C bus
     while (I2CMasterBusy (I2C0_BASE));

    // I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);   //send the send signal to send the register value
     I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);   //send the send signal to send the register value
     while (I2CMasterBusy (I2C0_BASE));

     I2CMasterSlaveAddrSet (I2C0_BASE, addr, true);  //set the master to read from the device
     while (I2CMasterBusy (I2C0_BASE));

     I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);    //send the receive signal to the device
     while (I2CMasterBusy (I2C0_BASE));

     return I2CMasterDataGet (I2C0_BASE);    //return the data read from the bus
 }
 // I2C0 initialized to use
 void I2C0_Write(uint8_t slave_addr, uint8_t reg, uint8_t data) {
     I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false); // get the address of the MPU6050
     I2CMasterDataPut(I2C0_BASE, reg); // put the specific register on SDA to read from
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); // send a start signal
     while(I2CMasterBusy(I2C0_BASE)); // wait until the I2C data sends

     I2CMasterDataPut(I2C0_BASE, data); // put the data to write on the register
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // finish sending signal
     while(I2CMasterBusy(I2C0_BASE));
 }

 // Initialize MPU6050 registers
 void MPU6050_init(void) {
     MPU_get_whom_am_I();
     I2C0_Write (MPU6050_ADDR, SMPLRT_DIV, 0x07); // gyro output sample rate = 2
     I2C0_Write (MPU6050_ADDR, PWR_MGMT_1, 0x01);
     I2C0_Write (MPU6050_ADDR, CONFIG, 0x00);
     I2C0_Write (MPU6050_ADDR, ACCEL_CONFIG, 0x01); // 2g scale range accel
     I2C0_Write (MPU6050_ADDR, GYRO_CONFIG, 0x18);
     I2C0_Write (MPU6050_ADDR, INT_ENABLE, 0x00);
 }

 int main(void) {



     /* Configure the system clock for 120 MHz */
     systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                           SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                           120000000);
     /* Initialize serial console */
     ConfigureUART(systemClock);
     I2C0_init();
     MPU6050_init(); // initialize MPU6050 to turn on
     // Local variables
     float xAccRaw, yAccRaw, zAccRaw; // raw values accelerometer
     float xGyrRaw, yGyrRaw, zGyrRaw; // raw values gyroscope


     while(1) {
         // Read raw data values from accelerometer
        xAccRaw = (I2C0_Read (MPU6050_ADDR, ACCEL_XOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, ACCEL_XOUT_L) );
        yAccRaw = (I2C0_Read (MPU6050_ADDR, ACCEL_YOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, ACCEL_YOUT_L) );
        zAccRaw = (I2C0_Read (MPU6050_ADDR, ACCEL_ZOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, ACCEL_ZOUT_L) );

        // Read raw data values from gyroscope
        xGyrRaw = (I2C0_Read (MPU6050_ADDR, GYRO_XOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, GYRO_XOUT_L) );
        yGyrRaw = (I2C0_Read (MPU6050_ADDR, GYRO_YOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, GYRO_YOUT_L) );
        zGyrRaw = (I2C0_Read (MPU6050_ADDR, GYRO_ZOUT_H) << 8 | I2C0_Read(MPU6050_ADDR, GYRO_ZOUT_L) );


        // Accelerometer values displayed
        UARTprintf("Accelerometer: { X: %s, ", xAccRaw);
        SysCtlDelay(5000000);
     }
 }

 // read slave device
 void MPU_get_whom_am_I(void) {
     uint8_t WAI; // clear register
     I2C0_Write(MPU6050_ADDR, PWR_MGMT_1, 0x00);
     while(I2CMasterBusy(I2C0_BASE));
     WAI = I2C0_Read(MPU6050_ADDR, WHO_AM_I);
     while(I2CMasterBusy(I2C0_BASE));

     if (WAI != 0x68) {
         UARTprintf("Device Not Found\n");
         while(1);
     }
     else
         UARTprintf("Device Found\n");
     SysCtlDelay(50000000);
 }

 // Initialize I2C0
 void I2C0_init(void){
     /* Enable clocks to GPIO Port G and configure pins as I2C */
     MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
     while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)))
     {
     }

     MAP_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
     MAP_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
     MAP_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
     MAP_GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);

     /* Enable the clock to I2C-1 module and configure the I2C Master */
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

 }


