#!/bin/bash
#
# This is the kernel build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

echo -e "\033[;31mBuild kernel start...\033[0m"

build_compress_kernel()
{
	cd $BSP_DIR/system/compressed
	make clean
	make -j8
}

build_raw_kernel()
{
	cd $BSP_DIR

	# clean temp files
	rm -rf rtthread* kernel.dis System.map rtthread.bin

	# let kservice.c recompiled, to update date and time in rt_show_version
	rm -f $TOP_DIR/out/build/kernel/src/kservice.o

	scons -j8
}

cur_dir=`pwd`

#
# step1: build raw img
#
build_raw_kernel
if [ $? -ne 0 ]; then
	echo -e "\033[;34mBuild raw img fail!\033[0m"
	cd $cur_dir
	exit -1
fi
echo -e "\033[;31mBuild raw img success!\033[0m"

#
# step2: build compressed img
#
build_compress_kernel
if [ $? -ne 0 ]; then
	echo -e "\033[;34mBuild compressed img fail!\033[0m"
	cd $cur_dir
	exit -1
fi
echo -e "\033[;31mBuild compressed img success!\033[0m"

echo -e "\033[;31mBuild kernel success!\033[0m"

