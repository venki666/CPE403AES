#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Users/venkim/ti/tirtos_tivac_2_16_00_08/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages
override XDCROOT = /Users/venkim/ti/xdctools_3_32_01_22_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Users/venkim/ti/tirtos_tivac_2_16_00_08/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;/Users/venkim/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;/Users/venkim/ti/xdctools_3_32_01_22_core/packages;..
HOSTOS = MacOS
endif
