################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source" --include_path="/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-2033156511: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/Applications/ti/ccs1010/ccs/utils/sysconfig_1.5.0/sysconfig_cli.sh" -s "/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/.metadata/product.json" -o "syscfg" --compiler ccs "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-2033156511 ../empty.syscfg
syscfg/ti_drivers_config.c: build-2033156511
syscfg/ti_drivers_config.h: build-2033156511
syscfg/ti_utils_build_linker.cmd.exp: build-2033156511
syscfg/syscfg_c.rov.xs: build-2033156511
syscfg/: build-2033156511

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source" --include_path="/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/Users/venkim/workspace_v10/empty_test_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


