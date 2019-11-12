# invoke SourceDir generated makefile for app.pem3
app.pem3: .libraries,app.pem3
.libraries,app.pem3: package/cfg/app_pem3.xdl
	$(MAKE) -f C:\Users\venkim\workspace_v9\sensorportable2_CC1350_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\venkim\workspace_v9\sensorportable2_CC1350_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

