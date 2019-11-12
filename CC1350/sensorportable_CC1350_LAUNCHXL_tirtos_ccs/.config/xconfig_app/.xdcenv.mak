#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc13x0_sdk_3_20_00_23/source;C:/ti/simplelink_cc13x0_sdk_3_20_00_23/kernel/tirtos/packages;C:/Users/venkim/workspace_v9/sensorportable_CC1350_LAUNCHXL_tirtos_ccs/.config
override XDCROOT = C:/ti/xdctools_3_51_03_28_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc13x0_sdk_3_20_00_23/source;C:/ti/simplelink_cc13x0_sdk_3_20_00_23/kernel/tirtos/packages;C:/Users/venkim/workspace_v9/sensorportable_CC1350_LAUNCHXL_tirtos_ccs/.config;C:/ti/xdctools_3_51_03_28_core/packages;..
HOSTOS = Windows
endif
