# invoke SourceDir generated makefile for TIRTOS_demo.pem4f
TIRTOS_demo.pem4f: .libraries,TIRTOS_demo.pem4f
.libraries,TIRTOS_demo.pem4f: package/cfg/TIRTOS_demo_pem4f.xdl
	$(MAKE) -f /Users/venkim/workspace_ccs/TivaC_A3T_EK_TM4C123GXL_TI2/src/makefile.libs

clean::
	$(MAKE) -f /Users/venkim/workspace_ccs/TivaC_A3T_EK_TM4C123GXL_TI2/src/makefile.libs clean

