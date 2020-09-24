################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-15037121:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-15037121-inproc

build-15037121-inproc: ../release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Applications/ti/ccs1010/xdctools_3_61_02_27_core/xs" --xdcpath="/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/source;/Users/venkim/ti/simplelink_cc13x2_26x2_sdk_4_20_00_35/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC1352 -r release -c "/Applications/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS" --compileOptions " -DDeviceFamily_CC13X2 " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-15037121 ../release.cfg
configPkg/compiler.opt: build-15037121
configPkg/: build-15037121


