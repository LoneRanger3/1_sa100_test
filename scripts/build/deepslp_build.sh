#!/bin/bash
#
# This is the deepsleep build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

echo -e "\033[;31mBuild deep sleep start...\033[0m"

if [ ! -f $BSP_DIR/pm/deepsleep/standby/Makefile ]; then
	echo -e "\033[;31mBuild deep sleep abort, the Makefile not exist!\033[0m"
	exit 0
fi

dir_org=`pwd`

#
# build standby
#
echo -e "\033[;36mBuild standby start...\033[0m"
cd $BSP_DIR/pm/deepsleep/standby
make clean && make
if [ $? -ne 0 ]; then
	cd $dir_org
	echo -e "\033[;34mBuild standby failed!\033[0m"
	exit -1
else
	echo -e "\033[;36mBuild standby success!\033[0m"
fi

#
# build deepsleep.suspend
#
echo -e "\033[;36mBuild suspend start...\033[0m"
cd $BSP_DIR/pm/deepsleep/suspend
make clean && make
if [ $? -ne 0 ]; then
	cd $dir_org
	echo -e "\033[;34mBuild suspend failed!\033[0m"
	exit -1
else
	echo -e "\033[;36mBuild suspend success!\033[0m"
fi

#
# build deepsleep.resume
#
echo -e "\033[;36mBuild resume start...\033[0m"
cd $BSP_DIR/pm/deepsleep/resume
make clean && make
if [ $? -ne 0 ]; then
	cd $dir_org
	echo -e "\033[;34mBuild resume failed!\033[0m"
	exit -1
else
	echo -e "\033[;36mBuild resume success!\033[0m"
fi

cd $dir_org
echo -e "\033[;31mBuild deep sleep success!\033[0m"
exit 0

