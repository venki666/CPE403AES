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
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
/* Driver Header files */
#include <ti/drivers/ADC.h>
#include <ti/display/Display.h>
/* Driver configuration */
#include "ti_drivers_config.h"

/* ADC sample count */
#define ADC_SAMPLE_COUNT  (10)

#define THREADSTACKSIZE   (768)

/* ADC conversion result variables */
uint16_t adcValue0;
uint32_t adcValue0MicroVolt;
uint16_t adcValue1[ADC_SAMPLE_COUNT];
uint32_t adcValue1MicroVolt[ADC_SAMPLE_COUNT];

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* 1 second delay */

    /* Call driver init functions */
    GPIO_init();
    ADC_init();
    Display_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();
    /* Open the display for output */
     Display_Handle hSerial = Display_open(Display_Type_UART, NULL);
    ADC_Handle   adc;
     ADC_Params   params;
     int_fast16_t res;

     ADC_Params_init(&params);
     adc = ADC_open(CONFIG_ADC_0, &params);

     if (adc == NULL) {
         Display_printf(hSerial, 1, 0, "Error initializing CONFIG_ADC_0\n");
         while (1);
     }
     /* Configure the LED pin */
     GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

     /* Turn on user LED */
     GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
     /* Blocking mode conversion */
     while(1)
     {
     res = ADC_convert(adc, &adcValue0);
     if (res == ADC_STATUS_SUCCESS) {
         Display_printf(hSerial, 1, 0, "CONFIG_ADC_0 raw result: %d\n", adcValue0);
         if(adcValue0 > 1000)
         {
             GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
         }
         else{
             GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
         }
     }
     else {
         Display_printf(hSerial, 1, 0, "CONFIG_ADC_0 convert failed\n");
       }
     usleep(500000);
     }
     return (NULL);
}
