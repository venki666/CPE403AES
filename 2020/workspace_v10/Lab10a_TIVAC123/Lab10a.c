#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#ifndef M_PI
#define M_PI                    3.14159265358979323846  // Pi
#endif

// Depth of the buffer
#define SERIES_LENGTH 1500

// Buffer to store the computed floating-point values
float gSeriesData[SERIES_LENGTH];

int32_t i32DataCount = 0;

int main(void)
{
    // Radian value
    float fRadians;

    ROM_FPULazyStackingEnable();    // Enables lazy stacking of floating-point registers
    ROM_FPUEnable();                // Enables the FPU since it's disabled by default

    // System clock set to 50 MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Formula to compute and scale the radian value
    fRadians = ((2 * M_PI) / SERIES_LENGTH);

    // Computes the sine wave
    while(i32DataCount < SERIES_LENGTH)
    {
        // Computes the given function
        gSeriesData[i32DataCount] = sinf(fRadians * 50 * i32DataCount) +
                                    0.5 * cosf(fRadians * 200 * i32DataCount);
        // Update the index to compute the next sine point
        i32DataCount++;
    }

    while(1)
    {
    }
}
