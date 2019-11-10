################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./iqmath_example.obj" \
"./startup_ccs.obj" \
"../blinky_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/tivaware_c_series_2_1_4_178/IQmath/ccs/IQmathLib/Debug/IQmathLib.lib" \
-l"C:/ti/tivaware_c_series_2_1_4_178/driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
IQMath_example.out \

EXE_OUTPUTS__QUOTED += \
"IQMath_example.out" \

BIN_OUTPUTS += \
IQMath_example.bin \

BIN_OUTPUTS__QUOTED += \
"IQMath_example.bin" \


# All Target
all: IQMath_example.out

# Tool invocations
IQMath_example.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual -z -m"blinky_ccs.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/lib" -i"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="IQMath_example_linkInfo.xml" --rom_model -o "IQMath_example.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

IQMath_example.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/bin/armhex"  -o "IQMath_example.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "iqmath_example.obj" "startup_ccs.obj" 
	-$(RM) "iqmath_example.d" "startup_ccs.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccs910/ccs/utils/tiobj2bin/tiobj2bin" "IQMath_example.out" "IQMath_example.bin" "C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/bin/armofd" "C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_19.6.0.STS/bin/armhex" "C:/ti/ccs910/ccs/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

