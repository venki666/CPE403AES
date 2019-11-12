/******************************************************************************

 @file uart_printf.c

 @brief This file contains the interface to allow printf through UART

 Group: WCS LPC
 Target Device: cc13x0

 ******************************************************************************
 
 Copyright (c) 2016-2019, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/

#include <Board.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include <stdint.h>

/******************************************************************************
 Local Constants
 *****************************************************************************/

#define UART_PRINTF_BUF_LEN      1024

/******************************************************************************
 Local Variables
 *****************************************************************************/
#if defined(BOARD_DISPLAY_USE_UART)
static uint8_t  uartPrintf_outArray[UART_PRINTF_BUF_LEN];
static uint16_t uartPrintf_head = 0;
static uint16_t uartPrintf_tail = 0;
UART_Handle hUart = NULL;
#endif /* BOARD_DISPLAY_USE_UART */

/******************************************************************************
 Public Functions
 *****************************************************************************/

/*!
 * @brief       Initializes the putchar hooks with the handle to the UART.
 *
 * @param       handle - UART driver handle to an initialized and opened UART.
 */
void UartPrintf_init(UART_Handle handle)
{
#if defined(BOARD_DISPLAY_USE_UART)
    hUart = handle;
#endif /* BOARD_DISPLAY_USE_UART */
}

/******************************************************************************
 System Hook Functions
 *****************************************************************************/

/*!
 * @brief   User supplied PutChar function.
 *          typedef Void (*SysCallback_PutchFxn)(Char);
 *
 *          This function is called whenever the System module needs
 *          to output a character.
 *
 *          This implementation fills a very basic ring-buffer, and relies
 *          on another function to flush this buffer out to UART.
 *
 *          Requires SysCallback to be the system provider module.
 *          Initialized via SysCallback.putchFxn = "&uartPrintf_putch"; in the
 *          TI-RTOS configuration script.
 *
 * @param   ch - Character
 *
 * @post    ::uartPrintf_head is incremented by one with wrap at
 *          UART_PRINTF_BUF_LEN if there is room.
 */
void UartPrintf_putch(char ch)
{
#if defined(BOARD_DISPLAY_USE_UART)
    /*
     uartPrintf_tail should never catch up with uartPrintf_head.
     Discard in-between bytes.
    */
    if ( ((uartPrintf_head + 1) % UART_PRINTF_BUF_LEN) == uartPrintf_tail )
    {
        return;
    }

    uartPrintf_outArray[uartPrintf_head] = ch;
    uartPrintf_head++;

    if (uartPrintf_head >= UART_PRINTF_BUF_LEN)
    {
        uartPrintf_head = 0;
    }
#endif /* BOARD_DISPLAY_USE_UART */
}

/*!
 * @brief   Printf-buffer flush function
 *
 *          In this implementation it is intended to be called by the
 *          Idle task when nothing else is running.
 *
 *          This is achieved by setting up the Idle task in the TI-RTOS
 *          configuration script like so:
 *
 *          var Idle = xdc.useModule('ti.sysbios.knl.Idle');
 *          Idle.addFunc('&uartPrintf_flush');
 *
 * @post    ::uartPrintf_tail is incremented to where uartPrintf_head
 *          was at the time the function was called.
 */
void UartPrintf_flush(void)
{
#if defined(BOARD_DISPLAY_USE_UART)
    /* Abort in case UART hasn't been initialized. */
    if(NULL == hUart)
    {
        return;
    }
    else
    {
        /* Lock head position to avoid race conditions */
        uint16_t curHead = uartPrintf_head;


        /* Find out how much data must be output, and how to output it. */
        bool needWrap = curHead < uartPrintf_tail;
        uint16_t outLen
            = needWrap ? (UART_PRINTF_BUF_LEN - uartPrintf_tail+curHead)
                            : (curHead - uartPrintf_tail);

        if(outLen)
        {
            if(needWrap)
            {
                UART_write(hUart, &uartPrintf_outArray[uartPrintf_tail],
                           UART_PRINTF_BUF_LEN - uartPrintf_tail);
                UART_write(hUart, uartPrintf_outArray, curHead);
            }
            else
            {
                UART_write(hUart, &uartPrintf_outArray[uartPrintf_tail],
                           outLen);
            }
        }

        uartPrintf_tail = curHead;
    }
#endif /* BOARD_DISPLAY_USE_UART */
}

