################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Drivers/ECC/ECCROMCC26XX.obj: C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/common/cc26xx/ecc/ECCROMCC26XX.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" --cmd_file="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/config/build_components.opt" --cmd_file="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/examples/rtos/CC1350_LAUNCHXL/blestack/simple_peripheral/tirtos/iar/stack/build_config.opt" --cmd_file="C:/Users/venkim/workspace_v10/simple_peripheral_cc1350lp_stack_FlashROM/TOOLS/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="C:/Users/venkim/workspace_v10/simple_peripheral_cc1350lp_app_FlashROM" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/devices/cc13x0" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/examples/rtos/CC1350_LAUNCHXL/blestack/simple_peripheral/src/app" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/icall/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/roles/cc26xx" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/roles" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/dev_info" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/simple_profile/cc26xx" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/simple_profile" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/common/cc26xx" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/heapmgr" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/controller/cc26xx/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/target/_common" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/target" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/target/_common/cc26xx" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/osal/src/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/services/src/sdata" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/services/src/saddr" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/icall/src/inc" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/rom" --include_path="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/boards/CC1350_LAUNCHXL" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --define=BOARD_DISPLAY_EXCLUDE_UART --define=CC1350_LAUNCHXL --define=CC13XX --define=DeviceFamily_CC13X0 --define=Display_DISABLE_ALL --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=POWER_SAVING --define=USE_ICALL --define=USE_CORE_SDK --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Drivers/ECC/$(basename $(<F)).d_raw" --obj_directory="Drivers/ECC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


