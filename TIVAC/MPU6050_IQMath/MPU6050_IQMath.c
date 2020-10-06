//Midterm1
//
//References:
//http://www.ti.com/lit/ug/spmu371d/spmu371d.pdf
//

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/hw_mpu6050.h"
#include "sensorlib/mpu6050.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "math.h"

//For Task3/Task4
#include "IQmath/IQmathLib.h"

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536
#define SAMPLE_RATE 0.01
#define M_PI 3.14159265359
#define dt 0.01// 10 ms sample rate!

void ComplementaryFilter(_iq16 accData[3], _iq16 gyrData[3], float *pitch, float *roll)
{
    float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    // Angle around the X-axis
    *pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the Y-axis
    *roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;

    // Compensate for drift with accelerometer data
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
        // Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;

        // Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
}

void InitUART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //enables UART module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //enables GPIO port a

    GPIOPinConfigure(GPIO_PA0_U0RX); //PA0 as RX pin
    GPIOPinConfigure(GPIO_PA1_U0TX); //PA1 as TX pin
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); //sets UART pin type

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC); //sets the clock source
    UARTStdioConfig(0, 115200, 16000000); //disables uartstdio, sets baud rate to 115200, uses clock
}

//
// A boolean that is set when a MPU6050 command has completed.
//
volatile bool g_bMPU6050Done;

//
// I2C master instance
//
tI2CMInstance g_sI2CMSimpleInst;

//
// The function that is provided by this example as a callback when MPU6050
// transactions have completed.
//
void MPU6050Callback(void *pvCallbackData, uint_fast8_t ui8Status)
{
    //
    // See if an error occurred.
    //
    if (ui8Status != I2CM_STATUS_SUCCESS)
    {
        //
        // An error occurred, so handle it here if required.
        //
    }
    //
    // Indicate that the MPU6050 transaction has completed.
    //
    g_bMPU6050Done = true;
}

void InitI2C0(void)
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //Configure the pin muxing for I2C0 functions on port B2 and B3
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //Select the I2C function for these pins
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 8000800;

    // Initialize the I2C master driver.
    I2CMInit(&g_sI2CMSimpleInst, I2C0_BASE, INT_I2C0, 0xff, 0xff, SysCtlClockGet());
}

// Interrupt for I2CM
void I2CM_IntHandler(void)
{
    I2CMIntHandler(&g_sI2CMSimpleInst);
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //use system clock
    InitI2C0(); //initializes the I2C
    InitUART(); //Initializes the UART

    //Local Variables
    float fAccel[3], fGyro[3];
    float xAccel, yAccel, zAccel;
    float xGyro, yGyro, zGyro;
    float pitch, roll;
    _iq16 QAccel[3];
    _iq16 QGyro[3];
    tMPU6050 sMPU6050;

    //
    // Initialize the MPU6050. This code assumes that the I2C master instance
    // has already been initialized.
    //
    g_bMPU6050Done = false;
    MPU6050Init(&sMPU6050, &g_sI2CMSimpleInst, 0x68, MPU6050Callback, &sMPU6050);
    while (!g_bMPU6050Done);

    //
    // Configure the MPU6050 for +/- 4 g accelerometer range.
    //

    //Settings for the Accelerometer
    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050,
                           MPU6050_O_ACCEL_CONFIG, // Accelerometer configuration
                           0x00, // No need to mask
                           MPU6050_ACCEL_CONFIG_AFS_SEL_4G, // Accelerometer full-scale range 4g
                           MPU6050Callback,
                           &sMPU6050);
    while (!g_bMPU6050Done);

    //Settings for the Gyroscope
    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050,
                           MPU6050_O_GYRO_CONFIG, // Gyroscope configuration
                           0x00, // No need to mask
                           MPU6050_GYRO_CONFIG_FS_SEL_250, // Gyro full-scale range +/- 250 degrees/sec
                           MPU6050Callback,
                           &sMPU6050);
    while (!g_bMPU6050Done);

    //Turns on power for Accelerometer & Gyroscope
    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050,
                           MPU6050_O_PWR_MGMT_1, // Power management 1 register
                           0x00, // No need to mask
                           0x00,//0x02 & MPU6050_PWR_MGMT_1_DEVICE_RESET,
                           MPU6050Callback,
                           &sMPU6050);
    while (!g_bMPU6050Done);

    //Turns on power for Accelerometer & Gyroscope
    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050,
                           MPU6050_O_PWR_MGMT_2, // Power management 2 register
                           0x00, // No need to mask
                           MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_1_25, //Wake up at 1.25 Hz
                           MPU6050Callback,
                           &sMPU6050);
    while (!g_bMPU6050Done);


    // Loop forever reading data from the MPU6050
    while (1)
    {
        //Wait for MPU6050
        g_bMPU6050Done = false;
        MPU6050DataRead(&sMPU6050, MPU6050Callback, &sMPU6050);
        while (!g_bMPU6050Done);

        //Obtain raw values of Accelerometer and Gyroscope
        MPU6050DataAccelGetFloat(&sMPU6050, &fAccel[0], &fAccel[1], &fAccel[2]);
        MPU6050DataGyroGetFloat(&sMPU6050, &fGyro[0], &fGyro[1], &fGyro[2]);

        //Place raw values of Accelerometer and Gyroscope into a register
        //Added a multiple of a 1000 to make it more visible over the terminal
        xAccel = fAccel[0] * 1000;
        yAccel = fAccel[1] * 1000;
        zAccel = fAccel[2] * 1000;
        xGyro = fGyro[0] * 1000;
        yGyro = fGyro[1] * 1000;
        zGyro = fGyro[2] * 1000;

        //UART print to terminal
        UARTprintf("Acceleration (x,y,z): (%d, %d, %d)\n", (int)xAccel, (int)yAccel, (int)zAccel);
        UARTprintf("Gyroscope (x,y,z): (%d, %d, %d)\n", (int)xGyro, (int)yGyro, (int)zGyro);

        //IQMath Conversion
        QAccel[0] = _IQ16(fAccel[0]);
        QAccel[1] = _IQ16(fAccel[1]);
        QAccel[2] = _IQ16(fAccel[2]);
        QGyro[0] = _IQ16(fGyro[0]);
        QGyro[1] = _IQ16(fGyro[1]);
        QGyro[2] = _IQ16(fGyro[2]);

        ComplementaryFilter(QAccel, QGyro, &pitch, &roll);
        UARTprintf("Pitch: %d\nRoll: %d\n\n", (int)(pitch*10), (int)(roll*10)); //Scaled it to make it easier to view on the terminal

        //System clock delay
        SysCtlDelay(15000000);
    }
}


