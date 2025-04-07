#!/bin/bash
#
# This is the cppcheck scripts just used for git commit
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

arr=()
for arg in $*
do
    temp=${arg%/*}
    # avoid path: sdk root & eos/bsp/lombo
    if [ "x$temp" != "xeos/bsp/lombo" ] && [[ $temp =~ '/' ]]; then
        arr[${#arr[*]}]=$temp
    fi
done
arr=($(echo ${arr[*]} | sed 's/ /\n/g' | sort | uniq))

if [ ${#arr[@]} == 0 ];
then
	echo "----------------------------------"
	echo -e "\033[;31mcppcheck commit success! slips path.\033[0m"
	echo "----------------------------------"
	exit 0
fi

#echo -e "\033[;31mcppcheck ${arr[@]} start...\033[0m"

dir_org=`pwd`
cd eos/bsp/lombo/

scons -j4 cppcheck_dir="${arr[*]}" ; \

res=$?
if [ $res -eq 0 ]
then
        cd $dir_org
        echo "----------------------------------"
        echo -e "\033[;31mcppcheck commit success!\033[0m"
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
