## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/image_decode_io1_dm365_xv5T.ov5T \
  package/cfg/image_decode_io1_dm365_xv5T.xdl
	$(SED) 's"^\"\(package/cfg/image_decode_io1_dm365_xv5Tcfg.cmd\)\"$""\"/home/qxd/dm365/dvsdk_dm365_4_02_00_06/dmai_2_20_00_15/packages/ti/sdo/dmai/apps/image_decode_io1/linux/image_decode_io1_dm365_config/\1\""' package/cfg/image_decode_io1_dm365_xv5T.xdl > $@
