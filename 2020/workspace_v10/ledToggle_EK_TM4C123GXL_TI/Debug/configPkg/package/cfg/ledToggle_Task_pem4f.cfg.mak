# invoke SourceDir generated makefile for ledToggle_Task.pem4f
ledToggle_Task.pem4f: .libraries,ledToggle_Task.pem4f
.libraries,ledToggle_Task.pem4f: package/cfg/ledToggle_Task_pem4f.xdl
	$(MAKE) -f /Users/venkim/workspace_ccs/ledToggle_EK_TM4C123GXL_TI/src/makefile.libs

clean::
	$(MAKE) -f /Users/venkim/workspace_ccs/ledToggle_EK_TM4C123GXL_TI/src/makefile.libs clean

