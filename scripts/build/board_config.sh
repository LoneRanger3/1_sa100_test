#!/bin/bash
#
# This is the top build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

# write build environments to .build
write_env()
{
	# Bakup old config
	if [ -f $TOP_DIR/.build ]; then
		mv $TOP_DIR/.build $TOP_DIR/.build.old
	fi

	echo "export LOMBO_CPU=$LOMBO_CPU" >> $TOP_DIR/.build
	echo "export LOMBO_SOC=$LOMBO_SOC" >> $TOP_DIR/.build
	echo "export LOMBO_BOARD=$LOMBO_BOARD" >> $TOP_DIR/.build
	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
		echo "export LOMBO_STORAGE=$LOMBO_STORAGE" >> $TOP_DIR/.build
		echo "export LOMBO_DDR_SIP=$LOMBO_DDR_SIP" >> $TOP_DIR/.build
	fi
	echo "" >> $TOP_DIR/.build

	echo "export TOP_DIR=$TOP_DIR" >> $TOP_DIR/.build
	echo "export SCRIPT_DIR=$TOP_DIR/scripts" >> $TOP_DIR/.build
	echo "export SCRIPT_BUILD=$TOP_DIR/scripts/build" >> $TOP_DIR/.build
	echo "export KERN_DIR=$TOP_DIR/eos" >> $TOP_DIR/.build
	echo "export BSP_DIR=$TOP_DIR/eos/bsp/lombo" >> $TOP_DIR/.build
	echo "export BSP_PLAT=$BSP_DIR/platform" >> $TOP_DIR/.build
	echo "export BSP_CFG=$BSP_DIR/config" >> $TOP_DIR/.build
	echo "export PACK_DIR=$TOP_DIR/pack" >> $TOP_DIR/.build
	echo "export PACK_PLAT=$PACK_DIR/platform" >> $TOP_DIR/.build
	echo "export PACK_TOOLS=$PACK_DIR/tools" >> $TOP_DIR/.build
	echo "export PACK_LIB=$PACK_DIR/tools/lib" >> $TOP_DIR/.build
	echo "" >> $TOP_DIR/.build

	echo "export BSP_PLAT_CPU=$BSP_PLAT/$LOMBO_CPU" >> $TOP_DIR/.build
	echo "export BSP_PLAT_SOC=$BSP_PLAT/$LOMBO_CPU/$LOMBO_SOC" >> $TOP_DIR/.build
	echo "export BSP_CFG_CPU=$BSP_CFG/$LOMBO_CPU" >> $TOP_DIR/.build
	echo "export BSP_CFG_SOC=$BSP_CFG/$LOMBO_CPU/$LOMBO_SOC" >> $TOP_DIR/.build
	echo "export PACK_PLAT_CPU=$PACK_PLAT/$LOMBO_CPU" >> $TOP_DIR/.build
	echo "export PACK_PLAT_SOC=$PACK_PLAT/$LOMBO_CPU/$LOMBO_SOC" >> $TOP_DIR/.build
	echo "export PACK_PLAT_BOARD=$PACK_PLAT/$LOMBO_CPU/$LOMBO_SOC/$LOMBO_BOARD" >> $TOP_DIR/.build
	if [ "x$LOMBO_CPU" != "xn7" ] || [ "x$LOMBO_SOC" != "xv0" ]; then
		echo "export PACK_OUT=$PACK_PLAT/$LOMBO_CPU/$LOMBO_SOC/$LOMBO_BOARD/files/out" >> $TOP_DIR/.build
	else
		echo "export PACK_OUT=$PACK_PLAT/$LOMBO_CPU/$LOMBO_SOC/$LOMBO_BOARD/files" >> $TOP_DIR/.build
	fi

	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "cdr" ]; then
		echo "export FAST_BOOT=$FAST_BOOT" >> $TOP_DIR/.build
	fi

	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "tdr" ]; then
		echo "export FAST_BOOT=$FAST_BOOT" >> $TOP_DIR/.build
	fi

	if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "sar" ]; then
		echo "export FAST_BOOT=$FAST_BOOT" >> $TOP_DIR/.build
	fi
}

# update defconfig file
updte_defconfig()
{
	BSP_DEF_CFG=$BSP_CFG/$LOMBO_CPU/$LOMBO_SOC/lombo_${LOMBO_CPU}${LOMBO_SOC}_${LOMBO_BOARD}_defconfig
	DOT_CFG=$BSP_DIR/.config

	if [ -f "$DOT_CFG" ]; then
		mv $DOT_CFG $BSP_DIR/.config.old
	fi

	#echo "copy $BSP_DEF_CFG to $DOT_CFG"
	cp $BSP_DEF_CFG $DOT_CFG
}

item_select()
{
	cnt=0
	sel_num=0

	for item in `ls $2`
	do
		sel[$cnt]=$item
		sel_prompt[$cnt]="$cnt: "$item
		echo ${sel_prompt[$cnt]}
		((cnt++))
	done

	read -p "$1 select: " sel_num

	if [ "x$sel_num" == "x"		\
		-o $sel_num -ge $cnt	\
		-o $sel_num -lt 0 ] ; then
		echo -e "\033[;31m err: input invalid for $1\033[0m"
		exit -1
	fi
}

storage_select()
{
cat <<-eof
0. MMC(SD/EMMC)
1. SPI Nor
2. SPI Nand
eof

	read -p "storage type select: " sel_num
	if [ "x$sel_num" == "x" ]; then
		echo -e "\033[;31m error: input invalid for storage\033[0m"
		exit -1
	fi

	if [ $sel_num -eq 0 ]; then
		LOMBO_STORAGE=mmc
	elif [ $sel_num -eq 1 ]; then
		LOMBO_STORAGE=nor
	elif [ $sel_num -eq 2 ]; then
		LOMBO_STORAGE=spi_nand
	else
		echo -e "\033[;31m error: input invalid for storage\033[0m"
		exit -1
	fi
}

ddr_sip_select()
{
	cat <<-eof
	0. normal
	1. sip
	eof

	read -p "ddr encapsulation type: " sip
	if [ "x$sip" == "x" ]; then
		echo -e "\033[;31m error: input invalid \033[0m"
		exit -1
	fi

	if [ $sip -eq 0 ]; then
		LOMBO_DDR_SIP=0
	elif [ $sip -eq 1 ]; then
		LOMBO_DDR_SIP=1
	else
		echo -e "\033[;31m error: input invalid \033[0m"
		exit -1
	fi
}

fast_boot_select()
{
cat <<-eof
0. no
1. yes (can only boot from nor)
eof

	read -p "fast boot select: " FAST_BOOT
	if [ "x$FAST_BOOT" == "x" ]; then
		echo -e "\033[;31m error: input invalid for fast boot\033[0m"
		exit -1
	fi
	echo "fast boot flag $FAST_BOOT"
}

#
# cpu select
#
item_select cpu $PACK_PLAT
LOMBO_CPU=${sel[$sel_num]}
TMP_DIR=$PACK_PLAT/$LOMBO_CPU

#
# IC select
#
item_select soc $TMP_DIR
LOMBO_SOC=${sel[$sel_num]}
TMP_DIR=$TMP_DIR/$LOMBO_SOC

#
# Board select
#
item_select board $TMP_DIR
LOMBO_BOARD=${sel[$sel_num]}
TMP_DIR=$TMP_DIR/$LOMBO_BOARD

#
# Storage select, only for n7v0, temporarily
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v0" ]; then
	storage_select
	ddr_sip_select
fi

#
# Boot mode select, only for n7v1 cdr
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "cdr" ]; then
	fast_boot_select
fi

#
# Boot mode select, only for n7v1 tdr
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "tdr" ]; then
	fast_boot_select
fi

# Boot mode select, only for n7v1 sar
#
if [ "$LOMBO_CPU" == "n7" ] && [ "$LOMBO_SOC" == "v1" ] && [ "$LOMBO_BOARD" == "sar" ]; then
	fast_boot_select
fi

# write to .build file
write_env

# updte .config
updte_defconfig

exit 0
