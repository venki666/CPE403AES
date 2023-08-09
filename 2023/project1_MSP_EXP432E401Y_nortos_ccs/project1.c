//*****************************************************************************
//
// project0.c - Example to demonstrate minimal Driverlib setup
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
//#include "ti/devices/msp432e4/driverlib/driverlib.h"
#include <ti/devices/msp432e4/inc/msp.h>

void led_init(){
    GPION->DIR|=BIT1;//D1 light on
    GPION->DEN|=BIT1;
}


int main(void)
{

    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12;               // activate clock for Port N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12)) == 0){}; // wait for preparation of Port N
    volatile uint32_t ui32Loop;
    led_init();


    while(1){
        GPION->DATA ^=BIT1;
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}
