#!/bin/bash
#
# This is the top build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

SCRIPT_BUILD=$(cd `dirname $0`; pwd)
#echo "SCRIPT_BUILD=$SCRIPT_BUILD"
build=$SCRIPT_BUILD/../../.build

show_help()
{
        printf "\nbuild.sh - Top level build scritps\n"
        printf "Valid Options:\n"
        printf "  help    - show help message\n"
        printf "  config  - config board\n"
        printf "  clean   - clean board config and kernel\n"
        printf "  pack    - generate package\n\n"
}

set_env()
{
        . $build
        #show_board

        if [ ! -d "$BSP_PLAT_SOC"       \
                -o ! -d "$BSP_CFG_SOC"  \
                -o ! -d "$PACK_PLAT_SOC" ]; then
                echo -e "\033[;31m err: .build invalid, please config first\033[0m"
                exit -1
        fi
}

if [ "x$1" == "x--help" ]; then
	echo "Cppcheck help:"
	echo "---------------------------------------------------"
	echo -e "./cppcheck.sh \\t\\t\\tcheck all projects,eg:./cppcheck.sh"
	echo -e "./cppcheck.sh project-dir\\tcheck a project,eg:\\n \
\\t\\t\\t\\t\\t./cppcheck.sh eos/bsp/lombo/applications"
	echo "---------------------------------------------------"
	echo -e "All the reports will be saved in\
\033[1;34m eos/bsp/lombo/build/cppcheck_report\033[0m"
	exit -1
fi

if [ ! -f $build ]; then
        echo -e "\033[;31m err: .build not exist, please config first\033[0m"
        show_help
        exit -1
fi

#
# 1. set env
#
set_env

#
# 2. prebuild: to correct the .config, rtconfig.h, .build, defconfig
#
bash $SCRIPT_BUILD/pre_build.sh
if [ $? -ne 0 ]; then
	exit -1
fi

#
# 3. check projects
#
echo -e "\033[;31mcppcheck $1 start...\033[0m"

if [ "x$1" == "x" ]
then
	dir_pro=$(cd "$SCRIPT_BUILD/../.."; pwd)
elif [ -d $1 ]
then
	if [ "x${1: -1}" == "x/" ]
	then
		dir_pro=${1%/*}
	else
		dir_pro=$1
	fi
else
	echo "----------------------------------"
	echo -e "\033[;34mcppcheck fail! '$1' does not exist.\033[0m"
	echo "----------------------------------"
	exit -1
fi

dir_org=`pwd`
cd $SCRIPT_BUILD/../../eos/bsp/lombo/

scons -j4 cppcheck_dir=$dir_pro ; \

res=$?
if [ $res -eq 0 ]
then
        cd $dir_org
        echo "----------------------------------"
        echo -e "\033[;31mcppcheck $1 success!\033[0m"
        echo "----------------------------------"
        exit 0
else
        cd $dir_org
        if [ $res -eq 2 ]
        then
            echo "----------------------------------"
            echo -e "\033[;34mcppcheck commit pass!\033[0m"
            echo "----------------------------------"
            exit 0
        else
            echo "----------------------------------"
            echo -e "\033[;34mcppcheck commit fail!\033[0m"
            echo "----------------------------------"
            exit -1
        fi
fi
