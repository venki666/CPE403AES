
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

void I2C0_Write (uint8_t addr, uint8_t N, ...)
//Writes data from master to slave
//Takes the address of the device, the number of arguments, 
//and a variable amount of register addresses to write to
{
    I2CMasterSlaveAddrSet (I2C0_BASE, addr, false); //Find the device based on the address given
    while (I2CMasterBusy (I2C0_BASE));

    va_list vargs;  //variable list to hold the register addresses passed

    va_start (vargs, N);    //initialize the variable list with the number of arguments

    I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));   
    //put the first argument in the list in to the I2C bus
    while (I2CMasterBusy (I2C0_BASE));
    if (N == 1) //if only 1 argument is passed, send that register command then stop
    {
        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy (I2C0_BASE));
        va_end (vargs);
    }
    else
    //if more than 1, loop through all the commands until they are all sent
    {
        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while (I2CMasterBusy (I2C0_BASE));
        uint8_t i;
        for (i = 1; i < N - 1; i++)
        {
            I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));   
            //send the next register address to the bus
            while (I2CMasterBusy (I2C0_BASE));

            I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);   
            //burst send, keeps receiving until the stop signal is received
            while (I2CMasterBusy (I2C0_BASE));
        }

        I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));   
        //puts the last argument on the SDA bus
        while (I2CMasterBusy (I2C0_BASE));

        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); 
        //send the finish signal to stop transmission
        while (I2CMasterBusy (I2C0_BASE));

        va_end (vargs);
    }

}
