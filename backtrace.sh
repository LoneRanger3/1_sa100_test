#!/bin/bash
#
# This is the backtrace scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

echo -e "\033[;31mBacktrace start...\033[0m"

ADD2LINE=tools/toolchain/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-addr2line
ELFILE=eos/bsp/lombo/rtthread-lombo.elf

if [[ "x$1" == "x" || ! -f $1 ]]; then
	echo "please input a file"
	exit -1
fi

if [ ! -f $ADD2LINE ]; then
	echo -e "\033[;31mExtract toolchain start...\033[0m"

	cd tools/toolchain
	tar -xvf gcc-arm-none-eabi-7-2017-q4-major.tar.bz2
	cd -

	echo -e "\033[;31mExtract toolchain finished!\n\033[0m"
fi

# should use unix format
dos2unix $1 >/dev/null

printf "\033[;32m  %-40s%s\n\033[0m" "===FUNCTION===" "========LINE========"

val=null
while read line
do
	#echo "get line $line"

	if [[ "x$line" == "x" || "${line: -1}" != "]" ]]; then
		continue
	fi

	#echo "get valid line $line"

	val=${line##*[}
	val=${val%%]*}
	line=`$ADD2LINE -e $ELFILE -f $val 2>/dev/null`
	#echo "line $line"

	if [[ $line == *[\/]* ]]; then
		line=`echo $line | sed -e 's/[[:space:]]//g' `
		line=`echo $line | sed -e 's/^[\t\n]$//g' `
		func=${line%%/*}
		line=${line#*/}
		line="/$line"
		printf "    %-38s%s\n" $func $line
		if [[ "$func" == "rt_thread_exit" ]]; then
			printf "\n"
		fi
	fi
done < $1

echo -e "\033[;31mBacktrace end\033[0m"
