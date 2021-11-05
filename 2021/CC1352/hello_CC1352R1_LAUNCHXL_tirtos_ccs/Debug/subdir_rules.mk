################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-651001087:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-651001087-inproc

build-651001087-inproc: ../hello.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Users/venkatesanmuthukumar/ti/xdctools_3_62_00_08_core/xs" --xdcpath="/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source;/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC13X2_CC26X2 -r release -c "/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs\" --include_path=\"/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug\" --include_path=\"/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source\" --include_path=\"/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source/ti/posix/ccs\" --include_path=\"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-651001087 ../hello.cfg
configPkg/compiler.opt: build-651001087
configPkg/: build-651001087

build-2061426609: ../hello.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/Users/venkatesanmuthukumar/ti/sysconfig_1_8_2/sysconfig_cli.sh" -s "/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/.metadata/product.json" --script "/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/hello.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-2061426609 ../hello.syscfg
syscfg/ti_drivers_config.c: build-2061426609
syscfg/ti_drivers_config.h: build-2061426609
syscfg/ti_utils_build_linker.cmd.genlibs: build-2061426609
syscfg/syscfg_c.rov.xs: build-2061426609
syscfg/ti_utils_runtime_model.gv: build-2061426609
syscfg/ti_utils_runtime_Makefile: build-2061426609
syscfg/: build-2061426609

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs" --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source" --include_path="/Users/venkatesanmuthukumar/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/source/ti/posix/ccs" --include_path="/Applications/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/Users/venkatesanmuthukumar/workspace_v10/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


