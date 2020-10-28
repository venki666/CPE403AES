# invoke SourceDir generated makefile for hello.pem4f
hello.pem4f: .libraries,hello.pem4f
.libraries,hello.pem4f: package/cfg/hello_pem4f.xdl
	$(MAKE) -f /Users/venkim/workspace_ccs/hello_CC1352R1_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f /Users/venkim/workspace_ccs/hello_CC1352R1_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

