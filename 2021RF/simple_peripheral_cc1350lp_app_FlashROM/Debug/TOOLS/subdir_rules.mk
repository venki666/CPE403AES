################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-272016337:
	@$(MAKE) --no-print-directory -Onone -f TOOLS/subdir_rules.mk build-272016337-inproc

build-272016337-inproc: C:/ti/simplelink_cc13x0_sdk_4_10_01_01/examples/rtos/CC1350_LAUNCHXL/blestack/simple_peripheral/tirtos/ccs/config/app_ble.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_51_03_28_core/xs" --xdcpath="C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source;C:/ti/simplelink_cc13x0_sdk_4_10_01_01/kernel/tirtos/packages;C:/ti/ccs1011/ccs/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1350F128 -r release -c "C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS" --compileOptions "-mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path=\"C:/Users/venkim/workspace_v10/simple_peripheral_cc1350lp_app_FlashROM\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/devices/cc13x0\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/examples/rtos/CC1350_LAUNCHXL/blestack/simple_peripheral/src/app\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/icall/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/roles/cc26xx\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/roles\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/dev_info\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/simple_profile/cc26xx\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/profiles/simple_profile\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/common/cc26xx\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/heapmgr\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/controller/cc26xx/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/target/_common\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/target\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/target/_common/cc26xx\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/hal/src/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/osal/src/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/services/src/sdata\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/services/src/saddr\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/icall/src/inc\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/blestack/rom\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/boards/CC1350_LAUNCHXL\" --include_path=\"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include\" --define=BOARD_DISPLAY_EXCLUDE_UART --define=CC1350_LAUNCHXL --define=CC13XX --define=DeviceFamily_CC13X0 --define=Display_DISABLE_ALL --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=POWER_SAVING --define=USE_ICALL --define=USE_CORE_SDK --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-272016337 C:/ti/simplelink_cc13x0_sdk_4_10_01_01/examples/rtos/CC1350_LAUNCHXL/blestack/simple_peripheral/tirtos/ccs/config/app_ble.cfg
configPkg/compiler.opt: build-272016337
configPkg/: build-272016337


