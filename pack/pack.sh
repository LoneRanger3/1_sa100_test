#!/bin/bash
#
# This is the pack scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

export LD_LIBRARY_PATH=$PACK_LIB:$LD_LIBRARY_PATH

#
# create variable $PKG_NAME
#
date=`date +%F`
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
	if [ $LOMBO_DDR_SIP -eq 1 ]; then
		PKG_NAME=venus_$LOMBO_CPU$LOMBO_SOC\_$LOMBO_BOARD\_$LOMBO_STORAGE\_sip_$date.pkg
	else
		PKG_NAME=venus_$LOMBO_CPU$LOMBO_SOC\_$LOMBO_BOARD\_$LOMBO_STORAGE\_$date.pkg
	fi
else
	PKG_NAME=venus_$LOMBO_CPU$LOMBO_SOC\_$LOMBO_BOARD\_$date.pkg
fi

#
# create variable $BOOSTER_RAW, $LEAVER_INI..
#
cur_dir=`pwd`
PACK_INI=./pack.ini
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
	if [ $LOMBO_DDR_SIP -eq 1 ]; then
		BOOSTER_RAW=./files/booster_sip_raw.img
		PARAM_CFG=./files/param_sip.cfg
	else
		BOOSTER_RAW=./files/booster_raw.img
		PARAM_CFG=./files/param.cfg
	fi
	LEAVER_INI=./files/leaver.ini
	BOOSTER_IMG=./files/booster.img
	LEAVER_IMG=./files/leaver.img
else
	BOOSTER_RAW=./files/booster_raw.img
	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ]; then
		if [ "$LOMBO_BOARD" == "cdr" ] || [ "$LOMBO_BOARD" == "tdr" ] || [ "$LOMBO_BOARD" == "sar" ]; then
			if [ "$FAST_BOOT" == "1" ]; then
				BOOSTER_RAW=./files/booster_raw_fast.img
			fi
		fi
	fi
	PARAM_CFG=./files/config/param.cfg
	LEAVER_INI=./files/leaver/leaver.ini
	BOOSTER_IMG=./files/out/booster.img
	LEAVER_IMG=./files/out/leaver.img
fi

gen_kernel_img()
{
	$PACK_TOOLS/mkimage -A arm -O eos -C none -T kernel -a 0x40008000 -e 0x40008000 \
		-n 'eos v1.0' -d $BSP_DIR/rtthread-z.bin $PACK_OUT/kernel.img

#	gzip -k -f $BSP_DIR/rtthread.bin
#	$PACK_TOOLS/mkimage -A arm -O eos -C gzip -T kernel -a 0x40008000 -e 0x40008000 \
#		-n 'eos v1.0' -d $BSP_DIR/rtthread.bin.gz $PACK_OUT/kernel.img
}

gen_imbr()
{
	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
		$PACK_TOOLS/iMBR ./files/imbr_$LOMBO_STORAGE.ini ./files/imbr.bin
	else
		$PACK_TOOLS/iMBR ./files/leaver/imbr_mmc.ini ./files/out/imbr_mmc.bin
		$PACK_TOOLS/iMBR ./files/leaver/imbr_nand.ini ./files/out/imbr_nand.bin
		$PACK_TOOLS/iMBR ./files/leaver/imbr_nor.ini ./files/out/imbr_nor.bin
		$PACK_TOOLS/iMBR ./files/leaver/imbr_spi_nand.ini ./files/out/imbr_spi_nand.bin
	fi
}

gen_booster_img()
{
	$PACK_TOOLS/boot_data 0 $BOOSTER_RAW booster_out.img 0 0 0 0 8K 0 $PARAM_CFG DRAM CPU
}

gen_leaver_boot_img()
{
	cat $PACK_PLAT_BOARD/files/leaver/leaver_boot-nocfg.img $BSP_DIR/drivers/cfg/config.bin > $PACK_OUT/leaver_boot-cfg.img

	$PACK_TOOLS/mkimage -A arm -O u-boot -T firmware -C none -a 0x43000000 \
		-e 0 -n "U-Boot 2016.11 for lombo board" -z leaver \
		-d $PACK_OUT/leaver_boot-cfg.img $PACK_OUT/leaver_boot.img
}

gen_leaver_env_img()
{
	$PACK_TOOLS/mkenvimage -s 0x20000 -o $PACK_OUT/leaver_env.img ./files/leaver/env.cfg
}

gen_pkg()
{
	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
		$PACK_TOOLS/CometPack $PACK_INI $PKG_NAME
	else
		$PACK_TOOLS/CometPack $LEAVER_INI $LEAVER_IMG
		$PACK_TOOLS/CometPack $PACK_INI $PKG_NAME
	fi
}

rm_tmp_files()
{
	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
		rm -rf ./files/imbr*.bin
	else
		rm -rf ./files/out/imbr*.bin
	fi
	rm -rf $BOOSTER_IMG
	rm -rf $LEAVER_IMG
	#rm -rf bootfs.img rootfs.img kernel.img resume.bin
}

echo -e "\033[;31mpack $PKG_NAME start...\033[0m"

#
# 1. create $PACK_OUT dir if not exist
#
if [ "x$PACK_OUT" != "x" ] && [ ! -d $PACK_OUT ]; then
	echo "create dir: $PACK_OUT"
	mkdir $PACK_OUT
fi

#
# 2. generate kernel.img and copy to $PACK_OUT dir
#
gen_kernel_img
if [ $? -ne 0 ]; then
	echo -e "\033[;31m gen kernel.img failed\033[0m"
	exit -1
fi

cd $PACK_PLAT_BOARD

#
# 3. generate imbr_xx.bin
#
gen_imbr
if [ $? -ne 0 ]; then
	echo -e "\033[;31m gen imbr.bin failed\033[0m"
	exit -1
fi

#
# 4. generate booster.img
#
gen_booster_img
if [ $? -ne 0 ]; then
	echo -e "\033[;31m gen booster.img failed\033[0m"
	exit -1
else
	mv booster_out.img $BOOSTER_IMG
fi

#
# 5. generate leaver_boot.img
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ]; then
	gen_leaver_boot_img
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m gen leaver_boot.img failed\033[0m"
		exit -1
	fi
fi


#
# 6. generate leaver_env.img
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ]; then
	gen_leaver_env_img
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m gen leaver_env.img failed\033[0m"
		exit -1
	fi
fi

#
# 7. generate pkg file
#
gen_pkg
if [ $? -eq 0 ]; then
	echo -e "\033[;31mpack $PKG_NAME ok\033[0m"
	rm_tmp_files
	cd $cur_dir
	exit 0
else
	echo -e "\033[;34mpack $PKG_NAME fail\033[0m"
	rm_tmp_files
	cd $cur_dir
	exit -1
fi
