################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Drivers/ECC/ECCROMCC26XX.obj: C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/common/cc26xx/ecc/ECCROMCC26XX.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" --cmd_file="C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/TOOLS/defines/ble5_host_test_cc13x2r1lp_app_FlashROM_Release.opt" --cmd_file="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/config/build_components.opt" --cmd_file="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/config/factory_config.opt" --cmd_file="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/examples/rtos/CC1352R1_LAUNCHXL/ble5stack/host_test/tirtos/build_config.opt"  -mv7M4 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app" --include_path="C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/Application" --include_path="C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/Startup" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/controller/cc26xx/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/rom" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/common/cc26xx" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/common/cc26xx/rcosc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/icall/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/profiles/dev_info" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/profiles/simple_profile" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/target/_common" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/target/_common/cc26xx" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/hal/src/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/heapmgr" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/icall/src/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/npi/src" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/osal/src/inc" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/services/src/saddr" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/ble5stack/services/src/sdata" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/common/cc26xx" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/common/nv" --include_path="C:/ti/simplelink_cc13xx_cc26xx_sdk_5_30_01_01/source/ti/devices/cc13x2_cc26x2" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 --define=FLASH_ROM_BUILD --define=NVOCMP_NWSAMEITEM=1 -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Drivers/ECC/$(basename $(<F)).d_raw" --include_path="C:/Users/venkim/workspace_v10/ble5_host_test_cc13x2r1lp_app/FlashROM_Release/syscfg" --obj_directory="Drivers/ECC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


