################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ccs/%.obj: ../ccs/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/venki/TIworkspace_v12/mpu6050_MSP_EXP432E401Y_nortos_ccs" --include_path="C:/ti/simplelink_msp432e4_sdk_4_20_00_12/source" --include_path="C:/ti/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_4_20_00_12/source/ti/net/bsd" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none --define=DeviceFamily_MSP432E4 --define=__MSP432E401Y__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="ccs/$(basename $(<F)).d_raw" --obj_directory="ccs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


