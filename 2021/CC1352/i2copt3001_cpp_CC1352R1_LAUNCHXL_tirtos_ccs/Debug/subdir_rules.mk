################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O3 --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-811820463: ../i2copt3001_cpp.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/Users/venkatesanmuthukumar/ti/sysconfig_1.10.0/sysconfig_cli.sh" -s "/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/.metadata/product.json" --script "/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/i2copt3001_cpp.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-811820463 ../i2copt3001_cpp.syscfg
syscfg/ti_drivers_config.c: build-811820463
syscfg/ti_drivers_config.h: build-811820463
syscfg/ti_utils_build_linker.cmd.genlibs: build-811820463
syscfg/syscfg_c.rov.xs: build-811820463
syscfg/ti_utils_runtime_model.gv: build-811820463
syscfg/ti_utils_runtime_Makefile: build-811820463
syscfg/: build-811820463

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O3 --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O3 --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13xx_cc26xx_sdk_5_30_00_56/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/venkatesanmuthukumar/workspace_v10/i2copt3001_cpp_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


