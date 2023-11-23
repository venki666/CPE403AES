################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../clock.cfg 

CMD_SRCS += \
../MSP_EXP432E401Y_TIRTOS.cmd 

SYSCFG_SRCS += \
../clock.syscfg 

C_SRCS += \
../clock.c \
./syscfg/ti_drivers_config.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./configPkg/ \
./syscfg/ 

C_DEPS += \
./clock.d \
./syscfg/ti_drivers_config.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./clock.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.exp \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"configPkg/" \
"syscfg/" 

OBJS__QUOTED += \
"clock.obj" \
"syscfg/ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_drivers_config.h" \
"syscfg/ti_utils_build_linker.cmd.exp" \
"syscfg/syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"clock.d" \
"syscfg/ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"configPkg/linker.cmd" \
"configPkg/compiler.opt" \
"syscfg/ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../clock.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../clock.syscfg" 


