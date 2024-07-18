
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "i2c.h"


void initI2C0(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	SysCtlDelay(3);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlDelay(3);

  GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);

  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

  I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
  }

  void readI2C(uint8_t slave_addr, uint8_t reg, int *data)
  {
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
    I2CMasterDataPut(I2C0_BASE, reg);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C0_BASE));
    *data = I2CMasterDataGet(I2C0_BASE);
  }


// Sends 1 byte over i2c
void writeI2C(uint8_t slave_addr, uint8_t reg, uint8_t data)
{
	I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
	I2CMasterDataPut(I2C0_BASE, reg);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
  while(I2CMasterBusy(I2C0_BASE));
  I2CMasterDataPut(I2C0_BASE, data);
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
  while(I2CMasterBusy(I2C0_BASE));
}

//sends an I2C command to the specified slave
void I2C0_Send(uint8_t slave_addr, uint8_t num_of_args, ...)
{
   // Tell the master module what address it will place on the bus when
   // communicating with the slave.
   I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
   //stores list of variable number of arguments
   va_list vargs;
   //specifies the va_list to "open" and the last fixed argument
   //so vargs knows where to start looking
   va_start(vargs, num_of_args);
   //put data to be sent into FIFO
   I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
   //if there is only one argument, we only need to use the
   //single send I2C function
   if(num_of_args == 1)
   {
     //Initiate send of data from the MCU
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
     // Wait until MCU is done transferring.
     while(I2CMasterBusy(I2C0_BASE));
     //"close" variable argument list
     va_end(vargs);
   }
   //otherwise, we start transmission of multiple bytes on the
   //I2C bus
   else
   {
     //Initiate send of data from the MCU
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
     // Wait until MCU is done transferring.
     while(I2CMasterBusy(I2C0_BASE));
     //send num_of_args-2 pieces of data, using the
     //BURST_SEND_CONT command of the I2C module
     unsigned char i;
     for(i = 1; i < (num_of_args - 1); i++)
     {
       //put next piece of data into I2C FIFO
       I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
       //send next data that was just placed into FIFO
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
       // Wait until MCU is done transferring.
       while(I2CMasterBusy(I2C0_BASE));
     }
     //put last piece of data into I2C FIFO
     I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
     //send next data that was just placed into FIFO
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
     // Wait until MCU is done transferring.
     while(I2CMasterBusy(I2C0_BASE));
     //"close" variable args list
     va_end(vargs);
   }
 }

 void I2C0_read(uint8_t slave_addr, uint8_t *RxData, uint8_t N)
 {
   uint8_t i;

   I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
   while (I2CMasterBusy(I2C0_BASE));

   if (N==1)
   {
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while (I2CMasterBusy(I2C0_BASE));
    RxData[0]=I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE));
  }
  else
  {
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while (I2CMasterBusy(I2C0_BASE));
    RxData[0]=I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE));

    for (i=1;i<(N-1);i++)
    {
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
     while (I2CMasterBusy(I2C0_BASE));
     RxData[i]=I2CMasterDataGet(I2C0_BASE);
     while (I2CMasterBusy(I2C0_BASE));
   }

   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
   while (I2CMasterBusy(I2C0_BASE));
   RxData[N-1]=I2CMasterDataGet(I2C0_BASE);
   while (I2CMasterBusy(I2C0_BASE));
 }
}

//sends an I2C command to the specified slave
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

//sends an I2C command to the specified slave
void I2C0_Read16(uint8_t slave_addr, uint8_t pointer_reg, uint16_t RxData )
{
    uint8_t data;
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
    I2CMasterDataPut(I2C0_BASE, pointer_reg);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    while(I2CMasterBusy(I2C0_BASE));
    //MSB first
    data = I2CMasterDataGet(I2C0_BASE);
    RxData = (uint16_t)(data << 8);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    while(I2CMasterBusy(I2C0_BASE));
    //LSB later
    data = I2CMasterDataGet(I2C0_BASE);
    RxData |= (uint16_t)(data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
  }
