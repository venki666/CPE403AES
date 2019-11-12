# invoke SourceDir generated makefile for release.pem3
release.pem3: .libraries,release.pem3
.libraries,release.pem3: package/cfg/release_pem3.xdl
	$(MAKE) -f C:\Users\venkim\workspace_v9\tirtos_builds_CC1350_LAUNCHXL_release_ccs/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\venkim\workspace_v9\tirtos_builds_CC1350_LAUNCHXL_release_ccs/src/makefile.libs clean

