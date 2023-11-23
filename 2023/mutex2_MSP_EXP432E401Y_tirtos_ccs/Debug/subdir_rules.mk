################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs" --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/Debug" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/ti/posix/ccs" --include_path="/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1802502881:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1802502881-inproc

build-1802502881-inproc: ../mutex.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/home/venkim/ti/xdctools_3_61_02_27_core/xs" --xdcpath="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source;/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.msp432:MSP432E401Y -r release -c "/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs\" --include_path=\"/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/Debug\" --include_path=\"/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source\" --include_path=\"/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include\" --include_path=\"/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/ti/posix/ccs\" --include_path=\"/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include\" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1802502881 ../mutex.cfg
configPkg/compiler.opt: build-1802502881
configPkg/: build-1802502881

build-474507249: ../mutex.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/home/venkim/ti/sysconfig_1.4.0/sysconfig_cli.sh" -s "/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/.metadata/product.json" -o "syscfg" "/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/mutex.syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-474507249 ../mutex.syscfg
syscfg/ti_drivers_config.h: build-474507249
syscfg/ti_utils_build_linker.cmd.exp: build-474507249
syscfg/syscfg_c.rov.xs: build-474507249
syscfg/: build-474507249

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs" --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/Debug" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include" --include_path="/home/venkim/ti/simplelink_msp432e4_sdk_4_20_00_12/source/ti/posix/ccs" --include_path="/home/venkim/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/home/venkim/TIworkspace_v12/mutex_MSP_EXP432E401Y_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


