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
#include "ti/devices/msp432e4/driverlib/driverlib.h"

void key_init(){
    GPIOJ->DIR&=~(BIT0+BIT1); //set as input
    GPIOJ->PUR|=BIT0+BIT1;  //Input set to pull-up resistor
    GPIOJ->DEN|=BIT0+BIT1;  //Digital enable
    GPIOJ->DATA|=BIT0+BIT1;//Output set as high
}
void key_interrupt_init(){

    GPIOJ->IS &=~BIT1;
    GPIOJ->IBE &=~BIT1;
    GPIOJ->IEV &=~BIT1;//==  MAP_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_1, GPIO_FALLING_EDGE);
    GPIOJ->IM |=BIT1;//  ==    MAP_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_1);
    MAP_IntEnable(INT_GPIOJ);
}
void led_init(){
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}

void GPIOJ_IRQHandler(void)
{
    if (GPIOJ->RIS & BIT1){
        GPION->DATA ^=BIT1;
    }
    GPION->DATA ^=BIT0;
    GPIOJ->ICR |=BIT1;
}

int main(void)
{

    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R8;               // activate clock for Port N,J
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R8)) == 0){}; // wait for preparation of Port N,J

    key_init();
    key_interrupt_init();
    led_init();


    while(1){// if switch is pressed, led light on
        //switch1 -> PJ0 -> control D2->PN0
        if ((GPIOJ->DATA) & BIT0)
            GPION->DATA&=~BIT0;
        else
            GPION->DATA|=BIT0;
    }
}
