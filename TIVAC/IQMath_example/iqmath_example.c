//*****************************************************************************
//
// project0.c - Example to demonstrate minimal TivaWare setup
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

#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"driverlib/sysctl.h"
#include"math.h"
#include"IQmath/IQmathLib.h"
//one radian in degree
#define DEGREE 57.2957795
//Declared as global variables to access in watch windowon ccs
double g_result_hypo;
double g_result_angle_alpha_R, g_result_angle_beta_R;
double g_result_angle_alpha_D, g_result_angle_beta_D;
int main(void)
{
//Local Variables
    double IQ_Num_2_Float_Mul_Const;
    _iq16 base, adj, hypo, angle_alpha_R, angle_beta_R, angle_alpha_D, angle_beta_D;
//Configure system clock to 80MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
//Set base of the right triangle to 3cm
    base = _IQ16(3);
//Set adjacent of the right triangle to 4cm
    adj = _IQ16(4);
//Computes the square root of A2 + B2 using IQ numbers
    hypo = _IQ16mag(base, adj);
//Results in radian, which is in IQ number format
    angle_alpha_R = _IQ16asin(_IQ16div(adj, hypo));
    angle_beta_R = _IQ16acos(_IQ16div(adj, hypo));
//Results in degree, which is in IQ number format
    angle_alpha_D = _IQ16mpy(angle_alpha_R, _IQ16(DEGREE));
    angle_beta_D = _IQ16mpy(angle_beta_R, _IQ16(DEGREE));
//Convert IQ number to double: multiply IQ number by 2-n
    IQ_Num_2_Float_Mul_Const = pow(2, -16);
    g_result_hypo = hypo * IQ_Num_2_Float_Mul_Const;
    g_result_angle_alpha_D = angle_alpha_D * IQ_Num_2_Float_Mul_Const;
    g_result_angle_alpha_R = angle_alpha_R * IQ_Num_2_Float_Mul_Const;
    g_result_angle_beta_D = angle_beta_D * IQ_Num_2_Float_Mul_Const;
    g_result_angle_beta_R = angle_beta_R * IQ_Num_2_Float_Mul_Const;
    while (1); //program halted here
}
