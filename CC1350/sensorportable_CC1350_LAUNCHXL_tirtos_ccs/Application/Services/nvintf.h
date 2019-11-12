/******************************************************************************

 @file  nvintf.h

 @brief Function pointer interface to the NV API

 Group: WCS, LPC, BTS
 Target Device: cc13x0

 ******************************************************************************
 
 Copyright (c) 2014-2019, Texas Instruments Incorporated
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
#ifndef NVINTF_H
#define NVINTF_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// Constants and definitions
//*****************************************************************************

// NV system ID codes
#define NVINTF_SYSID_NVDRVR 0
#define NVINTF_SYSID_ZSTACK 1
#define NVINTF_SYSID_TIMAC  2
#define NVINTF_SYSID_REMOTI 3
#define NVINTF_SYSID_BLE    4
#define NVINTF_SYSID_6MESH  5
#define NVINTF_SYSID_APP    6

// NV driver status codes
#define NVINTF_SUCCESS      0
#define NVINTF_FAILURE      1
#define NVINTF_CORRUPT      2
#define NVINTF_NOTREADY     3
#define NVINTF_BADPARAM     4
#define NVINTF_BADLENGTH    5
#define NVINTF_BADOFFSET    6
#define NVINTF_BADITEMID    7
#define NVINTF_BADSUBID     8
#define NVINTF_BADSYSID     9
#define NVINTF_NOTFOUND     10
#define NVINTF_LOWPOWER     11
#define NVINTF_BADVERSION   12

//*****************************************************************************
// Typedefs
//*****************************************************************************

/**
 * NV Item Identification structure
 */
typedef struct nvintf_itemid_t
{
    //! NV System ID - identifies system (ZStack, BLE, App, OAD...)
    uint8_t systemID;
    //! NV Item ID
    uint16_t itemID;
    //! NV Item sub ID
    uint16_t subID;
} NVINTF_itemID_t;

//! Function pointer definition for the NVINTF_initNV() function
typedef uint8_t (*NVINTF_initNV)(void *param);

//! Function pointer definition for the NVINTF_compactNV() function
typedef uint8_t (*NVINTF_compactNV)(uint16_t minBytes);

//! Function pointer definition for the NVINTF_createItem() function
typedef uint8_t (*NVINTF_createItem)(NVINTF_itemID_t id,
                                     uint32_t length,
                                     void *buffer );

//! Function pointer definition for the NVINTF_deleteItem() function
typedef uint8_t (*NVINTF_deleteItem)(NVINTF_itemID_t id);

//! Function pointer definition for the NVINTF_readItem() function
typedef uint8_t (*NVINTF_readItem)(NVINTF_itemID_t id,
                                   uint16_t offset,
                                   uint16_t length,
                                   void *buffer );

//! Function pointer definition for the NVINTF_writeItem() function
typedef uint8_t (*NVINTF_writeItem)(NVINTF_itemID_t id,
                                    uint16_t length,
                                    void *buffer );

//! Function pointer definition for the NVINTF_writeItemEx() function
typedef uint8_t (*NVINTF_writeItemEx)(NVINTF_itemID_t id,
                                      uint16_t offset,
                                      uint16_t length,
                                      void *buffer );

//! Function pointer definition for the NVINTF_getItemLen() function
typedef uint32_t (*NVINTF_getItemLen)(NVINTF_itemID_t id);

//! Structure of NV API function pointers
typedef struct nvintf_nvfuncts_t
{
    //! Initialization function
    NVINTF_initNV initNV;
    //! Compact NV function
    NVINTF_compactNV compactNV;
    //! Create item function
    NVINTF_createItem createItem;
    //! Delete NV item function
    NVINTF_deleteItem deleteItem;
    //! Read item function
    NVINTF_readItem readItem;
    //! Write item function
    NVINTF_writeItem writeItem;
    //! Write existing item function
    NVINTF_writeItemEx writeItemEx;
    //! Get item length function
    NVINTF_getItemLen getItemLen;
} NVINTF_nvFuncts_t;

//*****************************************************************************
//*****************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* NVINTF_H */

