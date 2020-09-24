# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f /Users/venkim/workspace_v10/empty_TIRTOS__EK_TM4C123GXL_TI/src/makefile.libs

clean::
	$(MAKE) -f /Users/venkim/workspace_v10/empty_TIRTOS__EK_TM4C123GXL_TI/src/makefile.libs clean

