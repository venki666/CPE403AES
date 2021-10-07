#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include <string.h>
#include "icm20948_def.h"

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void initI2C0(void)
{
    // Turn on I2C0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    // Reset I2C0
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    // Enable GPIOB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure GPIO SCL/SDA pins on PB2/PB3
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Set pins to I2C function
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and master I2C
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    // Clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

int16_t I2CRead16(uint8_t slave_addr, uint8_t reg)
{
    // Variable to hold Data
    int16_t data = 0;
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    // Register to Read
    I2CMasterDataPut(I2C0_BASE, reg);

    // Send control bit
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    // Wait for send to finish
    while(I2CMasterBusy(I2C0_BASE));

    // We will read from the device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    // Start the burst receive
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    // Wait for receive to finish
    while(I2CMasterBusy(I2C0_BASE));
    // Get the MSB and shift by 8
    data = I2CMasterDataGet(I2C0_BASE) << 8;

    // Continue for the final burst receive
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    // Wait for final receive to finish
    while(I2CMasterBusy(I2C0_BASE));
    // Get the LSB and or with MSB to get the full data
    data |= I2CMasterDataGet(I2C0_BASE);

    //return data pulled from the specified register
    return data;
}

void I2C0_Send16(uint8_t slave_addr, uint8_t pointer_reg, uint16_t TxData)
{
  uint8_t data;
  I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
  I2CMasterDataPut(I2C0_BASE, pointer_reg);
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
  while(I2CMasterBusy(I2C0_BASE));
  // MSB First
  data = (uint8_t)((TxData >> 8) & 0x00FF);
  I2CMasterDataPut(I2C0_BASE, data);
  while(I2CMasterBusy(I2C0_BASE));
  //LSB Later
  data = (uint8_t)(TxData  & 0x00FF);
  I2CMasterDataPut(I2C0_BASE, data);
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
  while(I2CMasterBusy(I2C0_BASE));
}

int8_t I2CRead(uint8_t slave_addr, uint8_t reg)
{
    // Variable to hold Data
    int8_t data = 0;
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    // Register to Read
    I2CMasterDataPut(I2C0_BASE, reg);

    // Send control bit
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    // Wait for send to finish
    while(I2CMasterBusy(I2C0_BASE));

    // We will read from the device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    // Start the burst receive
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    // Wait for receive to finish
    while(I2CMasterBusy(I2C0_BASE));
    // Get the MSB and shift by 8
    data = I2CMasterDataGet(I2C0_BASE) << 8;
    //return data pulled from the specified register
    return data;
}

void I2C_Send(uint8_t slave_addr, uint8_t pointer_reg, uint8_t TxData)
{
  I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
  I2CMasterDataPut(I2C0_BASE, pointer_reg);
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
  while(I2CMasterBusy(I2C0_BASE));

  I2CMasterDataPut(I2C0_BASE, TxData);
  while(I2CMasterBusy(I2C0_BASE));
}

/*reads the slave device*/
void ICM_get_whom_am_I()
{
    uint8_t data[1];
    I2C_Send(SLAVE_ADDRESS, ICM20948_REG_PWR_MGMT_1, 0x80);        //clear data
    _delay_ms(10);
    I2C_Send(SLAVE_ADDRESS, ICM20948_REG_PWR_MGMT_1, 0x01);        //set clock
    _delay_ms(10);  // Wait for 200 ms.
    data[0]=I2CRead(SLAVE_ADDRESS, ICM20948_REG_WHO_AM_I);         //check slave device
    if (data[0] != ICM20948_DEVICE_ID)                  //inform if device found
    UARTprintf("Device Not Found\n");
    else
    UARTprintf("Device Found\n");
}

/*Initializes the ICM20948 device*/
void Init_ICM()
{                                      
    UARTprintf("I2C Initialized\n");
    _delay_ms(2);                                       // Wait for 200 ms.
    ICM_get_whom_am_I();
}


int main(void)
{
    // Set clock to 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Enable UART peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure UART GPIO
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);

    //Init I2C
    initI2C0();
    Init_ICM();
    while()
    {}

}

