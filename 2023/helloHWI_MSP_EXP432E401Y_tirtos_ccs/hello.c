/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 */

/*
 *  ======== hello.c ========
 */


/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include "ti/devices/msp432e4/driverlib/driverlib.h"
/*
 *  ======== main ========
 */

void ButtonFunction(UArg arg) {
    GPIOIntClear(GPIO_PORTJ_BASE, GPIO_PIN_0);
    /* turn off led 1 */
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
}


int main()
{
    /* Call driver init functions */
    //Board_init();
    //GPIO_init();
    uint32_t strength;
    uint32_t pinType;


    // enable port N
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    // LED2
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    /* set pin gpio port to output */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    /*configure pad as standard pin with default output current*/
    GPIOPadConfigGet(GPIO_PORTN_BASE, GPIO_PIN_1, &strength, &pinType);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_1, strength, GPIO_PIN_TYPE_STD);

    /* turn oN led 1 */
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 1);

    /* configure switch 1 with pull up as input on GPIO_PIN_0 as pull-up pin */
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOPadConfigGet(GPIO_PORTJ_BASE, GPIO_PIN_0, &strength, &pinType);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, strength, GPIO_PIN_TYPE_STD_WPU);


    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    BIOS_start();
    return(0);
}
