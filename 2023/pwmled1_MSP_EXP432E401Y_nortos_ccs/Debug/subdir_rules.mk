################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/oem/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs" --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs/Debug" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/source" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/source/third_party/CMSIS/Include" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/kernel/nortos" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/kernel/nortos/posix" --include_path="/home/oem/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-615660753: ../pwmled1.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/home/oem/ti/ccs1240/ccs/utils/sysconfig_1.17.0/sysconfig_cli.sh" -s "/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/.metadata/product.json" --script "/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs/pwmled1.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-615660753 ../pwmled1.syscfg
syscfg/ti_drivers_config.h: build-615660753
syscfg/ti_utils_build_linker.cmd.exp: build-615660753
syscfg/syscfg_c.rov.xs: build-615660753
syscfg/: build-615660753

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/oem/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs" --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs/Debug" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/source" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/source/third_party/CMSIS/Include" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/kernel/nortos" --include_path="/home/oem/ti/simplelink_msp432e4_sdk_4_10_00_13/kernel/nortos/posix" --include_path="/home/oem/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/home/oem/workspace_v12/pwmled1_MSP_EXP432E401Y_nortos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


