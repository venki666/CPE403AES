/******************************************************************************

 @file  mac_user_config.h

 @brief This file contains user configurable variables for the MAC
        radio such as type of RF Front End used with the TI device,
        TX power table, etc. Please see below for more detail.

        To change the default values of configurable variables:
          - Include the followings in your application main.c file:
            #ifndef USE_DEFAULT_USER_CFG

            #include "mac_user_config.h"

            // MAC user defined configuration
            macUserCfg_t macUser0Cfg = MAC_USER_CFG;

            #endif // USE_DEFAULT_USER_CFG

        Note: User configurable variables except the elements of
              the power table are only used during the initialization
              of the MAC. Changing the values of these variables
              except power table elements after this will have no
              effect.

        Note: To use the default user configurable variables, define
              the preprocessor symbol USE_DEFAULT_USER_CFG in your
              application project.

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

#ifndef MAC_USER_CONFIG_H
#define MAC_USER_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */
#include <hal_types.h>
#include <chipinfo.h>

/* COPROCESSOR PROJECTS */
#if defined( COPROCESSOR )
#if  defined(DeviceFamily_CC13X0)
    /* coprocessor project should have all the configurations */
    #include <rf_patches/rf_patch_cpe_sl_longrange.h>
    #include <rf_patches/rf_patch_mce_sl_longrange.h>
    #include <rf_patches/rf_patch_rfe_sl_longrange.h>
    #include <rf_patches/rf_patch_cpe_genfsk.h>
    #include <rf_patches/rf_patch_rfe_genfsk.h>
#elif defined(DeviceFamily_CC13X2)
    #ifndef FEATURE_UBLE
    #include <rf_patches/rf_patch_cpe_prop.h>
    #else
    #include <rf_patches/rf_patch_cpe_multi_protocol.h>
    #endif
#else
#endif /* end of defined(DeviceFamily_CC13X0) */

/* REGULAR PROJECTS */
#elif !defined( LIBRARY )
#include "api_mac.h"
#include "config.h"
#if defined( MAC_INTERNAL )
#include "msa.h"
#endif

/* ToDo : added 150kbps if needed */
#if (CONFIG_PHY_ID == APIMAC_STD_US_915_PHY_1)
#define US_BAND_1
#elif (CONFIG_PHY_ID == APIMAC_STD_ETSI_863_PHY_3)
#define ETSI_BAND
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_CHINA_433_PHY_128)
#define CHINA_BAND
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_US_LRM_915_PHY_129)
#define US_BAND_1
#define SL_LRM
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_CHINA_LRM_433_PHY_130)
#define CHINA_BAND
#define SL_LRM
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_ETSI_LRM_863_PHY_131)
#define ETSI_BAND
#define SL_LRM
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_US_915_PHY_132)
#define US_BAND_3
#elif (CONFIG_PHY_ID == APIMAC_GENERIC_ETSI_863_PHY_133)
#define ETSI_BAND_2
#else
#error "***MAC USER CONFIG BUILD ERROR*** Unknown phy id!"
#endif /* end of CONFIG_PHY_ID */
/* there is no patch for CC1352 */
#if defined(DeviceFamily_CC13X0)
    #ifdef SL_LRM
    #include <rf_patches/rf_patch_cpe_sl_longrange.h>
    #include <rf_patches/rf_patch_mce_sl_longrange.h>
    #include <rf_patches/rf_patch_rfe_sl_longrange.h>
    #else
    #include <rf_patches/rf_patch_cpe_genfsk.h>
    #include <rf_patches/rf_patch_rfe_genfsk.h>
    #endif /* end of SL_LRM */
#elif defined(DeviceFamily_CC13X2)
    #ifndef FEATURE_UBLE
    #include <rf_patches/rf_patch_cpe_prop.h>
    #else
    #include <rf_patches/rf_patch_cpe_multi_protocol.h>
    #endif
#else
#endif /* end of defined(DeviceFamily_CC13X0) */

/* LIBRARY PROJECTS */
#else
#endif

#ifndef USE_DMM
#include <ti/drivers/rf/RF.h>
#else
#include "rf_mac_api.h"
#endif //USE_DMM

/*******************************************************************************
 * MACROS
 */

/* TX power table calculation */
//     15..8   | 7..6 | 5..0
//   tempCoeff |  GC  |  IB
//
#define TX_POUT_DPA( TC, GC, IB)                                         \
  {.rawValue = ((((TC) & 0xFF) << 8) | (((GC) & 0x03) << 6) | ((IB) & 0x3F)), .paType = RF_TxPowerTable_DefaultPA }

#define TX_POUT_HPA( power )                                         \
  {.rawValue = power, .paType = RF_TxPowerTable_HighPA }

/*******************************************************************************
 * CONSTANTS
 */

// Radio mode
#define RADIO_MODE_PROPRIETARY           0

// RF Front End Settings
// Note: The use of these values completely depends on how the PCB is laid out.
//       Please see Device Package and Evaluation Module (EM) Board below.
/* RF Front End Settings */
#define RF_FRONTEND_MODE_MASK            0x07
#define RF_FE_DIFFERENTIAL               0
#define RF_FE_SINGLE_ENDED_RFP           1
#define RF_FE_SINGLE_ENDED_RFN           2
#define RF_FE_ANT_DIVERSITY_RFP_FIRST    3
#define RF_FE_ANT_DIVERSITY_RFN_FIRST    4
#define RF_FE_SINGLE_ENDED_RFP_EXT_PINS  5
#define RF_FE_SINGLE_ENDED_RFN_EXT_PINS  6

#define RF_BIAS_MODE_MASK               0x08
#define RF_FE_INT_BIAS                   (0<<3)
#define RF_FE_EXT_BIAS                   (1<<3)

// RF Front End Mode and Bias Configuration

#if defined(CC1310EM_7ID) || defined( CC1350EM_7ID )
#define RF_FE_MODE_AND_BIAS (RF_FE_DIFFERENTIAL | RF_FE_INT_BIAS)
#elif defined( CC1350STK_7XS )
#define RF_FE_MODE_AND_BIAS           ( RF_FE_SINGLE_ENDED_RFN |             \
                                        RF_FE_EXT_BIAS)
#elif !defined(USE_DMM)
// CC1310EM_7XD_7793) || defined( CC1350LP_7XD ) || (CC1352DK_7XD) || (CC1352R1_LAUNCHXL) ||(CC1310DK_7XD)
#define RF_FE_MODE_AND_BIAS (RF_FE_DIFFERENTIAL | RF_FE_EXT_BIAS)
#endif

// RF FE IOD
#define RF_FE_IOD_NUM     3
#define RF_FE_IOD         {NULL, NULL, NULL}
#define RF_FE_IOD_VAL     {NULL, NULL, NULL}
// RF FE saturation and sensitivity
#define RF_FE_SATURATION  0
#define RF_FE_SENSITIVITY 0

/* Tx Power numbers */
#define TX_POWER_27_DBM                 27
#define TX_POWER_26_DBM                 26
#define TX_POWER_25_DBM                 25
#define TX_POWER_24_DBM                 24
#define TX_POWER_23_DBM                 23
#define TX_POWER_22_DBM                 22
#define TX_POWER_21_DBM                 21
#define TX_POWER_20_DBM                 20
#define TX_POWER_19_DBM                 19
#define TX_POWER_18_DBM                 18
#define TX_POWER_17_DBM                 17
#define TX_POWER_16_DBM                 16
#define TX_POWER_15_DBM                 15
#define TX_POWER_14_DBM                 14
#define TX_POWER_13_DBM                 13
#define TX_POWER_12_DBM                 12
#define TX_POWER_11_DBM                 11
#define TX_POWER_10_DBM                 10
#define TX_POWER_9_DBM                   9
#define TX_POWER_8_DBM                   8
#define TX_POWER_7_DBM                   7
#define TX_POWER_6_DBM                   6
#define TX_POWER_5_DBM                   5
#define TX_POWER_4_DBM                   4
#define TX_POWER_3_DBM                   3
#define TX_POWER_2_DBM                   2
#define TX_POWER_1_DBM                   1
#define TX_POWER_0_DBM                   0
#define TX_POWER_MINUS_3_DBM            -3
#define TX_POWER_MINUS_5_DBM            -5
#define TX_POWER_MINUS_6_DBM            -6
#define TX_POWER_MINUS_9_DBM            -9
#define TX_POWER_MINUS_10_DBM          -10
#define TX_POWER_MINUS_12_DBM          -12
#define TX_POWER_MINUS_15_DBM          -15
#define TX_POWER_MINUS_18_DBM          -18
#define TX_POWER_MINUS_20_DBM          -20
#define TX_POWER_MINUS_21_DBM          -21

/* Synth divider */
#define MAC_LO_DIVIDER_5                    5
#define MAC_LO_DIVIDER_10                   10

/* Chip Type */
typedef enum
{
    MAC_RADIO_CC1310    = 0,
    MAC_RADIO_CC1350    = 1,
    MAC_RADIO_CC1310_CC1190 = 2,
    MAC_RADIO_CC1352 = 0,
    MAC_RADIO_CC1312 = 1,
    MAC_RADIO_CC1352P = 2,
    MAC_RADIO_CCXXX2    = 0,
} macChipType_t;

/* HW Revision */
typedef enum
{
    MAC_HW_REV_21       = 0,
    MAC_HW_REV_20       = 1,
    MAC_HW_MAX_REVS
} macHwRevision_t;
//
// Device Package and Evaluation Module (EM) Board
//
// The device may come in more than one types of packages.
// For each package, the user may change how the RF Front End (FE) is
// configured. The possible FE settings are provided as a set of defines.
// (The user can also set the FE bias, the settings of which are also provided
// as defines.) The user can change the value of RF_FE_MODE_AND_BIAS to
// configure the RF FE as desired. However, while setting the FE configuration
// determines how the device is configured at the package, it is the PCB the
// device is mounted on (the EM) that determines how those signals are routed.
// So while the FE is configurable, how signals are used is fixed by the EM.
// As can be seen, the value of RF_FE_MODE_AND_BIAS is organized by the EM
// board as defined by EMs produced by Texas Instruments Inc. How the device
// is mounted, routed, and configured for a user product would of course be
// user defined, and the value of RF_FE_MODE_AND_BIAS would have to be set
// accordingly; the user could even dispense with the conditional board
// compiles entirely. So too with the usage of the Tx Power tables. As can be
// seen in mac_user_config.c, there are two tables, one for packages using a
// differential FE, and one for single-end. This too has been organized by TI
// defined EMs and would have to be set appropriately by the user.


/*******************************************************************************
 * TYPEDEFS
 */

typedef const RF_TxPowerTable_Entry macTxPwrVal_t;

PACKED_TYPEDEF_CONST_STRUCT
{
  macTxPwrVal_t *pTxPwrVals;
} macTxPwrTbl_t;

/* Alternate HAL Assert function pointer */
typedef void (*alternateHalAssertFp_t)(void);

/* Range Extender function pointer */
typedef void (*setRangeExtenderFp_t)(uint32_t);
typedef void (*antennaSettingFp_t)(uint8_t);
typedef void (*antennaPwrFp_t)(uint8_t);

typedef struct
{
    uint32_t               getHwRevision;     /* API to get HW revision */
    RF_Mode                *pRfPatchTable;     /* RF Core patch */
    uint32_t               *pRfDrvTblPtr;     /* RF Driver API table */
    uint32_t               *pCryptoDrvTblPtr; /* Crypto Driver API table */
    macTxPwrTbl_t             *pTxPwrTbl;        /* TX Power table */
    uint32_t               **pOverrides;      /* RF Radio Overrides table */
    uint8_t                *pLoDivider;       /* loDivier for Setup Command */
    uint8_t                rfFeModeBias;      /* RF frontend mode bias */
    uint8_t                phyId;             /* phy ID */
    uint8_t                last;              /* indicating last array */
    setRangeExtenderFp_t   pSetRE;            /* Range Extender Function Pointer */
    alternateHalAssertFp_t pAssertFP;         /* Assert Function Pointer */
    antennaSettingFp_t     pAntSwitchFP;    /* antenna switch setting   */
    antennaPwrFp_t         pAntPwrFP;         /* antenna power setting   */
} macUserCfg_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */
extern const uint32_t       macRfDriverTable[];
extern const uint32_t       macCryptoDriverTable[];

extern RF_Mode        rfPatchTable;
extern RF_Mode        rfPatchTable_fsk;
extern RF_Mode        rfPatchTable_slr;

extern const macTxPwrTbl_t     macTxPwrTbl[];
extern const macTxPwrTbl_t     txPwrTbl_subg_US[];
extern const macTxPwrTbl_t     txPwrTbl_subg_EU[];
extern const macTxPwrTbl_t     txPwrTbl_433[];

extern const uint32_t       *pOverridesTable[];
extern const uint32_t       *pOverridesTable_fsk[];
extern const uint32_t       *pOverridesTable_slr[];
extern const uint32_t       *pOverridesTable_fsk_433[];
extern const uint32_t       *pOverridesTable_slr_433[];
extern const uint32_t       *pOverridesTable_fsk_200k[];

extern const uint8_t        loDivider;
extern const uint8_t        loDivider_433;

#if !defined( COPROCESSOR )
#define MAC_USER_CFG                { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable,             \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)macTxPwrTbl,               \
                                      (uint32_t **)pOverridesTable,         \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      CONFIG_PHY_ID,                        \
                                      TRUE,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }
#else /* COPROCESSOR project */
#define MAC_USER_CFG_915            { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_fsk,         \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_US,       \
                                      (uint32_t **)pOverridesTable_fsk,     \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_STD_US_915_PHY_1,              \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_200K_915       { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_fsk,              \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_US,        \
                                      (uint32_t **)pOverridesTable_fsk_200k,  \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_US_915_PHY_132,        \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_863            { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_fsk,              \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_EU,        \
                                      (uint32_t **)pOverridesTable_fsk,  \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_STD_ETSI_863_PHY_3,            \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_200K_863       { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_fsk,              \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_EU,        \
                                      (uint32_t **)pOverridesTable_fsk_200k,  \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_ETSI_863_PHY_133,      \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_433            { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_fsk,              \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_433,           \
                                      (uint32_t **)pOverridesTable_fsk_433,     \
                                      (uint8_t *)&loDivider_433,            \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_CHINA_433_PHY_128,     \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_LRM_915         { (uint32_t)ChipInfo_GetHwRevision,    \
                                      (RF_Mode *)&rfPatchTable_slr,           \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_US,        \
                                      (uint32_t **)pOverridesTable_slr,      \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_US_LRM_915_PHY_129,    \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_LRM_433        { (uint32_t)ChipInfo_GetHwRevision,     \
                                      (RF_Mode *)&rfPatchTable_slr,           \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_433,           \
                                      (uint32_t **)pOverridesTable_slr_433,  \
                                      (uint8_t *)&loDivider_433,            \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_CHINA_LRM_433_PHY_130, \
                                      FALSE,                                \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }

#define MAC_USER_CFG_LRM_863         { (uint32_t)ChipInfo_GetHwRevision,    \
                                      (RF_Mode *)&rfPatchTable_slr,           \
                                      (uint32_t *)macRfDriverTable,            \
                                      (uint32_t *)macCryptoDriverTable,        \
                                      (macTxPwrTbl_t *)txPwrTbl_subg_EU,        \
                                      (uint32_t **)pOverridesTable_slr,      \
                                      (uint8_t *)&loDivider,                \
                                      RF_FE_MODE_AND_BIAS,                  \
                                      APIMAC_GENERIC_ETSI_LRM_863_PHY_131,  \
                                      TRUE,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL,                                 \
                                      NULL }


#if defined(DeviceFamily_CC13X0)
#define MAC_USER_CFG                { MAC_USER_CFG_915,                     \
                                      MAC_USER_CFG_863,                     \
                                      MAC_USER_CFG_433,                     \
                                      MAC_USER_CFG_200K_915,                \
                                      MAC_USER_CFG_200K_863,                \
                                      MAC_USER_CFG_LRM_915,                 \
                                      MAC_USER_CFG_LRM_433,                 \
                                      MAC_USER_CFG_LRM_863 }
#endif

#if defined(DeviceFamily_CC13X2)
#define MAC_USER_CFG                { MAC_USER_CFG_915,                     \
                                      MAC_USER_CFG_863,                     \
                                      MAC_USER_CFG_LRM_915,                 \
                                      MAC_USER_CFG_LRM_863 }
#endif

#endif

/*********************************************************************
 * FUNCTIONS
 */

/* mac.c */
void macSetUserConfig( macUserCfg_t *pUserCfg );

#ifdef __cplusplus
}
#endif

#endif /* MAC_USER_CONFIG_H */
