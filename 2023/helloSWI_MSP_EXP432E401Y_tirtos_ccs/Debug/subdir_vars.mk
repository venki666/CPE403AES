################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../hello.cfg 

CMD_SRCS += \
../MSP_EXP432E401Y_TIRTOS.cmd 

SYSCFG_SRCS += \
../hello.syscfg 

C_SRCS += \
../hello.c \
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
./hello.d \
./syscfg/ti_drivers_config.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./hello.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.exp \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"configPkg/" \
"syscfg/" 

OBJS__QUOTED += \
"hello.obj" \
"syscfg/ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_drivers_config.h" \
"syscfg/ti_utils_build_linker.cmd.exp" \
"syscfg/syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"hello.d" \
"syscfg/ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"configPkg/linker.cmd" \
"configPkg/compiler.opt" \
"syscfg/ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../hello.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../hello.syscfg" 


