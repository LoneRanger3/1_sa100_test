#!/bin/bash
#
# This is the pre build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

echo -e "\033[;31mPrebuild check start...\033[0m"

BSP_DEF_CFG=$BSP_CFG_SOC/lombo_${LOMBO_CPU}${LOMBO_SOC}_${LOMBO_BOARD}_defconfig
DOT_CFG=$BSP_DIR/.config
RTCONFIG_H=$BSP_DIR/rtconfig.h
KERN_TOOLS=$KERN_DIR/tools
cur_dir=`pwd`

#
# prepare toolchain
#
cd $BSP_DIR
EXEC_PATH=`python -c 'import rtconfig; print rtconfig.EXEC_PATH'`
if [ ! -d "${EXEC_PATH}" ]; then
	echo -e "\033[;31mExtract toolchain start...\033[0m"

	TOOLCHAIN_DIR=${EXEC_PATH%/*}
	TOOLCHAIN_DIR=${TOOLCHAIN_DIR%/*}

	# check if EXEC_PATH end with '/'
	if [[ $EXEC_PATH == */ ]]; then
		TOOLCHAIN_DIR=${TOOLCHAIN_DIR%/*}
	fi
	TOOLCHAIN_FILE=${EXEC_PATH#*$TOOLCHAIN_DIR}
	TOOLCHAIN_FILE=${TOOLCHAIN_FILE#*/}
	TOOLCHAIN_FILE=${TOOLCHAIN_FILE%%/*}
	TOOLCHAIN_FILE=${TOOLCHAIN_FILE}.tar.bz2

	cd $TOOLCHAIN_DIR
	tar -xvf $TOOLCHAIN_FILE

	echo -e "\033[;31mExtract toolchain finished!\n\033[0m"
fi

if [ ! -f "$DOT_CFG" ]; then
	cp $BSP_DEF_CFG $DOT_CFG
	if [ $? -ne 0 ]; then
		exit -1
	fi
fi

dot_time=`stat -c %Y $DOT_CFG`
hcfg_time=`stat -c %Y $RTCONFIG_H`

if [ ! -f "$RTCONFIG_H"	] || [ $hcfg_time -lt $dot_time ]; then
	echo "regen rtconfig.h from .config"
	cd $KERN_TOOLS
	cp $DOT_CFG .config
	python -c 'import menuconfig; menuconfig.mk_rtconfig(".config")'
	mv rtconfig.h $RTCONFIG_H
	rm .config
fi

cd $cur_dir

echo -e "\033[;31mPrebuild check end!\033[0m"
