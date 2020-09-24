#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source;/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/kernel/tirtos/packages
override XDCROOT = /Applications/ti/ccs1010/xdctools_3_61_02_27_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source;/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/kernel/tirtos/packages;/Applications/ti/ccs1010/xdctools_3_61_02_27_core/packages;..
HOSTOS = MacOS
endif
