//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
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
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include"IQmath/IQmathLib.h"
#define PI 3.14159265359

volatile float res;           // floating point variable to verify results


int main(void)
{
    _iq qA, qB, qC;          // Q variables using global type
    _iq8 q8A, q8B, q8C;      // Q variables using Q8 type
    _iq15 q15A, q15C;          // Q variables using Q16 type


    /* Basic global Q operations. */
    qA = _IQ(1.0);
    qB = _IQ(2.5);
    qC = qA + qB; res=_IQtoF(qC);                // 3.5 = 1.0 + 2.5
    qC = qC - _IQmpy2(qA); res=_IQtoF(qC);        // 1.5 = 3.5 - 2*(1.0)
    qC = _IQmpy(qB, qC); res=_IQtoF(qC);          // 3.75 = 2.5 * 1.5
    qC = _IQdiv(qC, qB); res=_IQtoF(qC);          // 1.5 = 3.75 / 2.5
    qC = _IQsqrt(qB); res=_IQtoF(qC);             // 1.58114 = sqrt(2.5)

    /* Trigonometric global Q operations. */
    qA = _IQ(PI/4.0);
    qB = _IQ(0.5);
    qC = _IQsin(qA); res=_IQtoF(qC);              // 0.70710 = sin(PI/4)
    qC = _IQcos(qA); res=_IQtoF(qC);              // 0.70710 = cos(PI/4)
    qC = _IQatan(qB); res=_IQtoF(qC);             // 0.46365 = atan(0.5)

    /* Exponential global Q operations. */
    qA = _IQ(2.71828);
    qB = _IQ(0.5);
    qC = _IQexp2(qA); res=_IQtoF(qC);              // 1.0 = ln(e)
    qC = _IQexp(qB); res=_IQtoF(qC);              // 1.6487 = e^0.5

    /* Basic explicit type Q8 operations. */
    q8A = _IQ8(1.0);
    q8B = _IQ8(2.5);
    q8C = q8A + q8B; res=_IQ8toF(q8C);           // 3.5 = 1.0 + 2.5
    q8C = q8C - _IQmpy2(q8A); res=_IQ8toF(q8C);   // 1.5 = 3.5 - 2*(1.0)
    q8C = _IQ8mpy(q8B, q8C); res=_IQ8toF(q8C);    // 3.75 = 2.5 * 1.5
    q8C = _IQ8div(q8C, q8B); res=_IQ8toF(q8C);    // 1.5 = 3.75 / 2.5
    q8C = _IQ8sqrt(q8B); res=_IQ8toF(q8C);        // 1.58114 = sqrt(2.5)

    /* Trigonometric explicit type Q15 operations. */
    q15A = _IQ15(PI/4.0);
    q15C = _IQ15sin(q15A); res=_IQ15toF(q15C);    // 0.70710 = sin(PI/4)
    q15C = _IQ15cos(q15A); res=_IQ15toF(q15C);    // 0.70710 = cos(PI/4)

    /* Explicit type Q8 to Global Q conversion with saturation check. */
    q8A = _IQ8(1.0);
    q8B = _IQ8(16.0);
    qC = _IQ8toIQ(_IQsat(q8A, _IQtoQ8(MAX_IQ_POS), _IQtoQ8(MAX_IQ_NEG)));
    res = _IQtoF(qC);    // _Q8(1.0) -> _Q(1.0) (q8A does not saturate)
    qC = _IQ8toIQ(_IQsat(q8B, _IQtoQ8(MAX_IQ_POS), _IQtoQ8(MAX_IQ_NEG)));
    res = _IQtoF(qC);    // _Q8(16.0) -> ~MAX_Q_POS (q8A saturates to maximum positive _Q value)

    return 0;
}
