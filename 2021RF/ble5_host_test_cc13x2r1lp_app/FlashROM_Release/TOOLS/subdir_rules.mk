################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-252967279:
	@$(MAKE) --no-print-directory -Onone -f TOOLS/subdir_rules.mk build-252967279-inproc

build-252967279-inproc: ../TOOLS/ble_release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs1011/xdctools_3_62_01_15_core/xs" --xdcpath="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source;C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/kernel/tirtos/packages;C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source;C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/kernel/tirtos/packages;C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC1352R1F3 -r release -c "C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M4 --code_state=16 -me -O4 --opt_for_speed=0 --include_path=\"C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app\" --include_path=\"C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/Application\" --include_path=\"C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/Startup\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/controller/cc26xx/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/rom\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/common/cc26xx\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/common/cc26xx/rcosc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/icall/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/profiles/dev_info\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/profiles/simple_profile\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/target/_common\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/target/_common/cc26xx\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/heapmgr\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/icall/src/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/npi/src\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/osal/src/inc\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/services/src/saddr\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/services/src/sdata\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/common/cc26xx\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/common/nv\" --include_path=\"C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/devices/cc13x2_cc26x2\" --include_path=\"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=DeviceFamily_CC13X2 --define=FLASH_ROM_BUILD --define=NVOCMP_NWSAMEITEM=1 -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-252967279 ../TOOLS/ble_release.cfg
configPkg/compiler.opt: build-252967279
configPkg/: build-252967279


