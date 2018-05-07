## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/encodets_xv5T.ov5T \
  package/cfg/encodets_xv5T.xdl
	$(SED) 's"^\"\(package/cfg/encodets_xv5Tcfg.cmd\)\"$""\"/mnt/hgfs/share/dvsdk_dm365_4_02_00_06/dvsdk-demos_4_02_00_01/dm365/encodets/encodets_config/\1\""' package/cfg/encodets_xv5T.xdl > $@
