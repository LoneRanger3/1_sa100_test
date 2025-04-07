#!/bin/bash
#
# This is the top build scripts
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#

export TOP_DIR=`pwd`
export SCRIPT_DIR=$TOP_DIR/scripts
export SCRIPT_BUILD=$TOP_DIR/scripts/build
export KERN_DIR=$TOP_DIR/eos
export BSP_DIR=$TOP_DIR/eos/bsp/lombo
export BSP_PLAT=$BSP_DIR/platform
export BSP_CFG=$BSP_DIR/config
export PACK_DIR=$TOP_DIR/pack
export PACK_PLAT=$PACK_DIR/platform
export PACK_TOOLS=$PACK_DIR/tools
export PACK_LIB=$PACK_DIR/tools/lib

show_board()
{
	echo "--------------LOMBO CONFIG-------------"
	a=0
	cat $TOP_DIR/.build | while read line
	do
		tmp_key=${line%%=*}
		tmp_key=${tmp_key##*\ }
		tmp_val=${line##*=}
		if [ -z "$tmp_key" -a -z "$tmp_val" ]; then
			printf "\n"
		else
			printf "      %-20s: %s\n" "$tmp_key" "$tmp_val"
		fi
		let a=a+1
	done
	echo "---------------------------------------"
}

set_env()
{
	. .build

#	if [ ! -d "$BSP_PLAT_SOC"	\
	if [ ! -d "$BSP_CFG_SOC"	\
		-o ! -d "$PACK_PLAT_SOC" ]; then
		echo -e "\033[;31m err: .build invalid, please config first\033[0m"
		exit -1
	fi
}

show_help()
{
	printf "\nbuild.sh - Top level build scritps\n"
	printf "Valid Options:\n"
	printf " help    - show help message\n"
	printf " config  - config board\n"
	printf " clean   - clean board config and kernel\n"
	printf " pack    - generate package\n"
	printf " --lib dir [clean]	- build or clean a dynamic library(.so)\n"
	printf " --slib dir [clean]	- build or clean a static library(.a)\n"
	printf " --app dir		- build a application(.app)\n"
	printf " --app dir clean	- clean a application(.app) target files\n"
	printf " --app dir all		- build all applications(.app),dir: apps dir\n"
	printf " --app dir cleanall	- clean all applications(.app),dir: apps dir\n"
	printf "\n"
}

clean_files()
{
	local CLEAN_FILES="../../../out .config rtthread* kernel.dis System.map $IMAGE_NAME"

	# remove all .o
	find $TOP_DIR/case $TOP_DIR/media $TOP_DIR/framework $TOP_DIR/net \
		$TOP_DIR/eos $TOP_DIR/out -name "*.o" | xargs rm -rf

	# remove kernel tmp files
	cd $BSP_DIR
	scons -c
	rm -rf $CLEAN_FILES
}

build_modules()
{
	path=`pwd`/$2

	# prepare env if need
	if [ ! -f $BSP_DIR/rtua.py ]; then
		cd $BSP_DIR

		# backup elf files
		for file in `ls *.elf 2>/dev/null`
		do
			echo "mv $file ${file}.bkup"
			mv "$file" "${file}.bkup"
		done

		scons --target=ua -s

		# recover elf files
		for file in `ls *.bkup 2>/dev/null`
		do
			echo "mv $file ${file%\.*}"
			mv "$file" "${file%\.*}"
		done
	fi

	# build library or application
	cd $BSP_DIR/libraries

	if [ "x$1" == "x--app" ]; then
		if [ "x$3" == "xclean" ]; then
			scons $path $1=$path -c
			if [ -d $path/../../out/apps ]; then
				if [ ! ${path##*/} ]; then
					var=${path%/*}
					rm -f $path/../../out/apps/${var##*/}.app
				else
					rm -f $path/../../out/apps/${path##*/}.app
				fi
			fi
		elif [ "x$3" == "xall" ]; then
		    for file in `ls $path`
			do
				if [ x"$file"!=x"." -a x"$file"!=x".." ]; then
					if [ -d "$path/$file" ]; then
						scons $path/$file $1=$path/$file -j8
						if [ $? -ne 0 ]; then
							echo -e "\033[;31m err: build app\033[0m"
							exit -1
						fi
						if [ -d $path/../out ]; then
							if [ ! -d $path/../out/apps ]; then
								mkdir $path/../out/apps
							fi
							dst_app=$path/$file/$file.app
							dst_dir=$path/../out/apps/
							if [ -f $dst_app ]; then
								cp $dst_app $dst_dir
							else
								return -1
							fi
						fi
					fi
				fi
			done
		elif [ "x$3" == "xcleanall" ]; then
			for file in `ls $path`
			do
				if [ x"$file"!=x"." -a x"$file"!=x".." ]; then
					if [ -d "$path/$file" ]; then
						scons $path/$file $1=$path/$file -c
						if [ -f $path/../out/apps/$file.app ]; then
							rm $path/../out/apps/$file.app
						fi
					fi
				fi
			done
			if [ -f $BSP_DIR/rtua.py ]; then
				rm $BSP_DIR/rtua.py
			fi
			if [ -f $BSP_DIR/rtua.pyc ]; then
				rm $BSP_DIR/rtua.pyc
			fi
		else
			scons $path $1=$path -j8
			if [ -d $path/../../out ]; then
				if [ ! -d $path/../../out/apps ]; then
					mkdir $path/../../out/apps
				fi
				if [ ! ${path##*/} ]; then
					var=${path%/*}
					dst_app=$var/${var##*/}.app
					dst_dir=$path/../../out/apps/
					if [ -f $dst_app ]; then
						cp $dst_app $dst_dir
					else
						return -1
					fi
				else
					dst_app=$path/${path##*/}.app
					dst_dir=$path/../../out/apps/
					if [ -f $dst_app ]; then
						cp $dst_app $dst_dir
					else
						return -1
					fi
				fi
			fi
		fi
	elif [ "x$1" == "x--mod" ]; then
		if [ "x$3" == "xclean" ]; then
			scons $path $1=$path -c
			if [ -d $path/../../out/mod ]; then
				if [ ! ${path##*/} ]; then
					var=${path%/*}
					rm -f $path../../out/mod/${var##*/}.mod
				else
					rm -f $path../../out/mod/${path##*/}.mod
				fi
			fi
		else
			scons $path $1=$path -j8
			if [ -d $path/../../out ]; then
				if [ ! -d $path/../../out/mod ]; then
					mkdir $path/../../out/mod
				fi
				if [ ! ${path##*/} ]; then
					var=${path%/*}
					dst_mod=$var/${var##*/}.mod
					dst_dir=$path/../../out/mod/
					if [ -f $dst_mod ]; then
						cp $dst_mod $dst_dir
					else
						return -1
					fi
				else
					dst_mod=$path/${path##*/}.mod
					dst_dir=$path/../../out/mod/
					if [ -f $dst_mod ]; then
						cp $dst_mod $dst_dir
					else
						return -1
					fi
				fi
			fi
		fi
	else
		# $1 is --slib or --slib
		if [ "x$3" == "xclean" ]; then
			scons $path $1=$path -c
		else
			scons $path $1=$path -j8
		fi
	fi

	return 0
}

sar_board()
{
	cat .build|grep "LOMBO_BOARD=sar" >/dev/null
	if [ $? -ne 0 ]; then
		return -1
	fi
	cat ./eos/bsp/lombo/rtconfig.h|grep "#define RT_SEL_CAR_VIDEO_CASE" >/dev/null
	if [ $? -ne 0 ]; then
		printf "\033[;32m warning: you need menuconfig sar and gui\033[0m\n"
		return -1
	fi
	return 0
}

cdr_board()
{
	cat .build|grep "LOMBO_BOARD=cdr" >/dev/null
	if [ $? -ne 0 ]; then
		return -1
	fi
	cat ./eos/bsp/lombo/rtconfig.h|grep "#define RT_SEL_CAR_VIDEO_CASE" >/dev/null
	if [ $? -ne 0 ]; then
		printf "\033[;32m warning: you need menuconfig cdr and gui\033[0m\n"
		return -1
	fi
	return 0
}

tdr_board()
{
	cat .build|grep "LOMBO_BOARD=tdr" >/dev/null
	if [ $? -ne 0 ]; then
		return -1
	fi
	cat ./eos/bsp/lombo/rtconfig.h|grep "#define RT_SEL_CAR_VIDEO_CASE" >/dev/null
	if [ $? -ne 0 ]; then
		printf "\033[;32m warning: you need menuconfig tdr and gui\033[0m\n"
		return -1
	fi
	return 0
}

sar_board()
{
	cat .build|grep "LOMBO_BOARD=sar" >/dev/null
	if [ $? -ne 0 ]; then
		return -1
	fi
	cat ./eos/bsp/lombo/rtconfig.h|grep "#define RT_SEL_CAR_VIDEO_CASE" >/dev/null
	if [ $? -ne 0 ]; then
		printf "\033[;32m warning: you need menuconfig sar and gui\033[0m\n"
		return -1
	fi
	return 0
}

build_sar_proj()
{
	build_modules --mod case/sar/mod/mod_media/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_cdr_proj mod_media\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/sar/mod/mod_dms/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_sar_proj mod_dms\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/sar/apps/ all
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_sar_proj apps\033[0m"
		exit -1
	fi
	return 0
}

build_tdr_proj()
{
	build_modules --mod case/tdr/mod/mod_media/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_tdr_proj mod_media\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/tdr/mod/mod_adas/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_tdr_proj mod_adas\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/tdr/mod/mod_bsd/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_tdr_proj mod_bsd\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/tdr/apps/ all
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_tdr_proj apps\033[0m"
		exit -1
	fi
	return 0
}

clean_sar_proj()
{
	build_modules --mod case/sar/mod/mod_media/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_tdr_proj mod_media\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/sar/mod/mod_dms/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_sar_proj mod_dms clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/sar/apps/ cleanall
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_sar_proj apps clean\033[0m"
		exit -1
	fi
	return 0
}

clean_tdr_proj()
{
	build_modules --mod case/tdr/mod/mod_media/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_tdr_proj mod_media clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/tdr/mod/mod_adas/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_tdr_proj mod_adas clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/tdr/mod/mod_bsd/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_tdr_proj mod_bsd clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/tdr/apps/ cleanall
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_tdr_proj apps clean\033[0m"
		exit -1
	fi
	return 0
}

build_cdr_proj()
{
	build_modules --mod case/cdr/mod/mod_media/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_cdr_proj mod_media\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/cdr/mod/mod_adas/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_cdr_proj mod_adas\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/cdr/mod/mod_bsd/
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_cdr_proj mod_bsd\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/cdr/apps/ all
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: build_cdr_proj apps\033[0m"
		exit -1
	fi
	return 0
}

clean_cdr_proj()
{
	build_modules --mod case/cdr/mod/mod_media/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_cdr_proj mod_media clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/cdr/mod/mod_adas/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_cdr_proj mod_adas clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --mod case/cdr/mod/mod_bsd/ clean
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_cdr_proj mod_bsd clean\033[0m"
		exit -1
	fi
	cd $TOP_DIR
	build_modules --app case/cdr/apps/ cleanall
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: clean_cdr_proj apps clean\033[0m"
		exit -1
	fi
	return 0
}


build_case_proj()
{
	sar_board
	if [ $? -eq 0 ]; then
		build_sar_proj
		return 0
	fi

	cdr_board
	if [ $? -eq 0 ]; then
		build_cdr_proj
		return 0
	fi

	tdr_board
	if [ $? -eq 0 ]; then
		build_tdr_proj
		return 0
	fi

	return 0
}

clean_case_proj()
{
	cdr_board
	if [ $? -eq 0 ]; then
		clean_cdr_proj
		return 0
	fi

	tdr_board
	if [ $? -eq 0 ]; then
		clean_tdr_proj
		return 0
	fi

	sar_board
	if [ $? -eq 0 ]; then
		clean_sar_proj
		return 0
	fi

	return 0
}

#
# 1. param check
#
if [ "x$1" == "xconfig" ]; then
	bash $SCRIPT_BUILD/board_config.sh
	if [ $? -ne 0 ]; then
		echo -e "\033[;31m err: config failed\033[0m"
		exit -1
	fi

	show_board
	exit 0
elif [ "x$1" == "xpack" ]; then
	set_env

	bash $PACK_DIR/pack.sh
	if [ $? -ne 0 ]; then
		exit -1
	fi
	exit 0
elif [ "x$1" == "xrootfs" ]; then
	set_env

	bash $PACK_PLAT_BOARD/rootfs.sh
	if [ $? -ne 0 ]; then
		exit -1
	fi
	exit 0
elif [ "x$1" == "xclean" ]; then
	clean_case_proj
	clean_files
	exit 0
elif [ "x$1" == "xhelp" ]; then
	show_help
	exit 0
elif [ ! -f .build ]; then
	echo -e "\033[;31m err: .build not exist, please \"./build.sh config\"\033[0m"
	show_help
	exit -1
fi

#
# 2. set env
#
set_env
printf "\033[;32m----- Start build $LOMBO_CPU-$LOMBO_SOC-$LOMBO_BOARD\033[0m -----\n"

#
# 3. prebuild: correct the .config, rtconfig.h, defconfig
#
bash $SCRIPT_BUILD/pre_build.sh
if [ $? -ne 0 ]; then
	exit -1
fi

#
# 4. build single library
#
if [ "x$1" == "x--lib" -o "x$1" == "x--slib" -o "x$1" == "x--app" -o "x$1" == "x--mod" ]; then
	build_modules $1 $2 $3
	exit 0
fi

#
# 5. build deep sleep, because deepslp.bin is included(by incbin) by kernel src
#
if [ "x$LOMBO_CPU" == "xn7" ]; then
	bash $SCRIPT_BUILD/deepslp_build.sh
	if [ $? -ne 0 ]; then
		exit -1
	fi
else
	echo "deep sleep not ready for $LOMBO_CPU$LOMBO_SOC, so skip it.."
fi

#
# 6 build kernel
#
bash $SCRIPT_BUILD/kernel_build.sh
if [ $? -ne 0 ]; then
	exit -1
fi

#
# 7. build cdr app and module
#
build_case_proj

#
# 8. build rootfs
#
bash $PACK_PLAT_BOARD/rootfs.sh
if [ $? -ne 0 ]; then
	exit -1
fi

#
# 9. pack to generate the image file
#
bash $PACK_DIR/pack.sh
if [ $? -ne 0 ]; then
	exit -1
fi

exit 0
