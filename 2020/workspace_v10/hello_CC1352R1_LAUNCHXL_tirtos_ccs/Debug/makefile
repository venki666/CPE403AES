################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt" 
GEN_CMDS__FLAG := -l"configPkg/linker.cmd" 

ORDERED_OBJS += \
"./syscfg/ti_devices_config.obj" \
"./syscfg/ti_drivers_config.obj" \
"./hello2.obj" \
"../cc13x2_cc26x2_tirtos.cmd" \
$(GEN_CMDS__FLAG) \
-lti_utils_build_linker.cmd.genlibs \
-l"ti/devices/cc13x2_cc26x2/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

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
hello_CC1352R1_LAUNCHXL_tirtos_ccs.out \

EXE_OUTPUTS__QUOTED += \
"hello_CC1352R1_LAUNCHXL_tirtos_ccs.out" \

BIN_OUTPUTS += \
hello_CC1352R1_LAUNCHXL_tirtos_ccs.hex \

BIN_OUTPUTS__QUOTED += \
"hello_CC1352R1_LAUNCHXL_tirtos_ccs.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "hello_CC1352R1_LAUNCHXL_tirtos_ccs.out"

dependents:
	-cd /Users/venkim/workspace_ccs/tirtos_builds_cc13x2_cc26x2_debug_ccs/Debug && $(MAKE) all

# Tool invocations
hello_CC1352R1_LAUNCHXL_tirtos_ccs.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) /Users/venkim/workspace_ccs/tirtos_builds_cc13x2_cc26x2_debug_ccs/Debug/tirtos_builds_cc13x2_cc26x2_debug_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"hello_CC1352R1_LAUNCHXL_tirtos_ccs.map" -i"/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_30_00_54/source" -i"/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_30_00_54/kernel/tirtos/packages" -i"/Users/venkim/workspace_ccs/hello_CC1352R1_LAUNCHXL_tirtos_ccs/Debug/syscfg/" -i"/Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="hello_CC1352R1_LAUNCHXL_tirtos_ccs_linkInfo.xml" --rom_model -o "hello_CC1352R1_LAUNCHXL_tirtos_ccs.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

hello_CC1352R1_LAUNCHXL_tirtos_ccs.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armhex"  -o "hello_CC1352R1_LAUNCHXL_tirtos_ccs.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "syscfg/ti_devices_config.obj" "syscfg/ti_drivers_config.obj" "hello2.obj" 
	-$(RM) "syscfg/ti_devices_config.d" "syscfg/ti_drivers_config.d" "hello2.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/Users/venkim/workspace_ccs/tirtos_builds_cc13x2_cc26x2_debug_ccs/Debug/tirtos_builds_cc13x2_cc26x2_debug_ccs.lib:

-include ../makefile.targets

