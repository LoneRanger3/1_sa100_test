#!/bin/bash
#
# This is the rootfs files list
# Copyright (c) 2016-2020 by Lombotech. All Rights Reserved.
#
# usage:
#    1. manually copy the files that permanent used to $ROOTFS_RAW_DIR dir
#    2. edit files_to_copy array, add the files or dirs that you want to copy. for example:
#           files_to_copy=(
#               "case/cdr/apps/home/home.c                :        /app/abc/"
#               "case/cdr/apps/home/*                     :        /bin/"
#               "case/cdr/apps/home/*.h                   :        /lib/def/ikj/"
#               "case/cdr/apps/home/                      :        /res/opq/"
#               "case/cdr/apps/file_explorer/*.app        :        /dir1/dir2/"
#           )
#    3. edit cmds_to_exec array, add commands that you want to be executed.
#           cmds_to_exec=(
#               "mkdir -p $ROOTFS_OUT_DIR/app/dir3"
#               "cp $TOP_DIR/media/test/pano_test.c $ROOTFS_OUT_DIR/"
#               "cp -rf $TOP_DIR/media/oscl/ $ROOTFS_OUT_DIR/dir4/"
#           )
#    4. note: the $files_to_copy and $cmds_to_exec will be parsed and executed automatically during building
#

ROOTFS_OUT_DIR=$TOP_DIR/out/target/$LOMBO_CPU/$LOMBO_SOC/$LOMBO_BOARD/rootfs
ROOTFS_RAW_DIR=$PACK_PLAT_BOARD/files/rootfs_raw
ROOTFS_IMG_FAT=$PACK_OUT/rootfs_fat.img
ROOTFS_IMG_CRAM=$PACK_OUT/rootfs_cram.img

IFS="!!"

files_to_copy=(
	"case/cdr/out/*		:	/"
	#"include/system/*			:	/bin/"
	#"eos/include/libc/*.h			:	/lib/def/ikj/"
	#"eos/tools/tools/			:	/res/opq/"
	#"case/cdr/apps/file_explorer/*.app	:	/dir1/dir2/"
)

cmds_to_exec=(
	#"mkdir -p $ROOTFS_OUT_DIR/app/dir3"
	#"cp $TOP_DIR/media/test/pano_test.c $ROOTFS_OUT_DIR/"
	#"cp -rf $TOP_DIR/media/oscl/ $ROOTFS_OUT_DIR/dir4/"
#	"cp -rf $TOP_DIR/tools/lbgio/lbgio.mo $ROOTFS_OUT_DIR/bin/lbgio.mo"
#	"cp -rf $TOP_DIR/tools/lbgio/lbgio_scripts/ $ROOTFS_OUT_DIR/etc/lbgio_scripts/"
)

dump_usage()
{
	printf "\n----- How to make rootfs -----\n"
	printf "    1. manually copy the files that permanent used to rootfs_raw dir\n"
        printf "    2. edit files_to_copy array, add the files or dirs that you want to copy. for example:\n"
	printf "           files_to_copy=(                                                          \n"
	printf "               \"case/cdr/apps/home/home.c                :        /app/abc/\"      \n"
	printf "               \"case/cdr/apps/home/*                     :        /bin/\"          \n"
	printf "               \"case/cdr/apps/home/*.h                   :        /lib/def/ikj/\"  \n"
	printf "               \"case/cdr/apps/home/                      :        /res/opq/\"      \n"
	printf "               \"case/cdr/apps/file_explorer/*.app        :        /dir1/dir2/\"    \n"
	printf "           )                                                                        \n"
	printf "    3. edit cmds_to_exec array, add commands that you want to be executed.          \n"
	printf "           cmds_to_exec=(                                                           \n"
	printf "               \"mkdir -p $ROOTFS_OUT_DIR/app/dir3\"                                \n"
	printf "               \"cp $TOP_DIR/media/test/pano_test.c $ROOTFS_OUT_DIR/\"              \n"
	printf "               \"cp -rf $TOP_DIR/media/oscl/ $ROOTFS_OUT_DIR/dir4/\"                \n"
	printf "           )                                                                        \n"
	printf "    4. note: the files_to_copy and cmds_to_exec will be parsed and executed automatically during building\n"
}

copy_files()
{
	for value in ${files_to_copy[@]}
	do
		src=`echo $value | cut -d ":" -f 1`
		src=`echo $src | sed 's/^[ \t]*//g'`
		src=`echo $src | sed 's/[ \t]*$//g'`

		dst=`echo $value | cut -d ":" -f 2`
		dst=`echo $dst | sed 's/^[ \t]*//g'`
		dst=`echo $dst | sed 's/[ \t]*$//g'`

		src=$TOP_DIR/$src
		dst=$ROOTFS_OUT_DIR/$dst

		#
		# support for "/.../*", so comment this
		#
		#if [ ! -e "$src" ]; then
		#	echo "err: $src not exist!"
		#	return -1
		#fi

		if [ ! -d "$dst" ]; then
			echo "create dir $dst"
			mkdir -p $dst
		fi

		echo "cp $src to $dst"
		cp -rf $src $dst
	done

	return 0
}

exec_cmds()
{
	for value in ${cmds_to_exec[@]}
	do
		`eval $value`
		if [ $? -ne 0 ]; then
			echo "err: cmd \"$value\" execute err!"
			return -1
		else
			echo "cmd \"$value\" execute success!"
		fi
	done

	return 0
}

remove_old_rootfs()
{
	if [ -d $ROOTFS_OUT_DIR ]; then
		echo "clean $ROOTFS_OUT_DIR/ at first"
		rm -rf $ROOTFS_OUT_DIR/*
	else
		mkdir -p $ROOTFS_OUT_DIR
	fi

	if [ -f $ROOTFS_IMG_FAT ]; then
		echo "remove old $ROOTFS_IMG_FAT at first"
		rm $ROOTFS_IMG_FAT
	fi

	if [ -f $ROOTFS_OUT_DIR/../rootfs_fat.img ]; then
		echo "remove old $ROOTFS_OUT_DIR/../rootfs_fat.img at first"
		rm $ROOTFS_OUT_DIR/../rootfs_fat.img
	fi

	if [ -f $ROOTFS_IMG_CRAM ]; then
		echo "remove old $ROOTFS_IMG_CRAM at first"
		rm $ROOTFS_IMG_CRAM
	fi

	if [ -f $ROOTFS_OUT_DIR/../rootfs_cram.img ]; then
		echo "remove old $ROOTFS_OUT_DIR/../rootfs_cram.img at first"
		rm $ROOTFS_OUT_DIR/../rootfs_cram.img
	fi
}

gen_roofs()
{
	#
	# 1. generate cramfs rootfs (for nor boot)
	#
	mkcramfs $ROOTFS_OUT_DIR $ROOTFS_OUT_DIR/../rootfs_cram.img
	if [ $? -ne 0 ]; then
		echo "err: mkcramfs \"$ROOTFS_OUT_DIR\" failed!"
		return -1
	else
		echo "mkcramfs \"$ROOTFS_OUT_DIR\" success!"
		cp $ROOTFS_OUT_DIR/../rootfs_cram.img $ROOTFS_IMG_CRAM
	fi

	#
	# 2. generate fat rootfs (for card boot)
	#
	if [ -e "rootfs_fat.img" ]; then
		echo "remove rootfs_fat.img first"
		rm rootfs_fat.img
	fi
	dd if=/dev/zero of=fsdata.img bs=64k count=1k
	mkdir tmpmnt/
	/sbin/mkfs.fat fsdata.img
	sudo mount -t vfat -o umask=0000 fsdata.img tmpmnt/
	cp -rf $ROOTFS_OUT_DIR/* tmpmnt/
	sudo umount tmpmnt/
	mv fsdata.img $ROOTFS_OUT_DIR/../rootfs_fat.img
	cp $ROOTFS_OUT_DIR/../rootfs_fat.img $ROOTFS_IMG_FAT
	rm -rf tmpmnt/

	return 0
}

echo -e "\033[;31mGenerate rootfs start...\033[0m"

#
# 1. remove old rootfs at first
#
remove_old_rootfs

#
# 2. copy rootfs_raw/* to rootfs output dir
#
if [ ! -d $ROOTFS_RAW_DIR ]; then
	echo "warn: $ROOTFS_RAW_DIR not exist!"
	#exit -1
else
	echo "cp $ROOTFS_RAW_DIR/* to $ROOTFS_OUT_DIR/"
	cp -rf $ROOTFS_RAW_DIR/* $ROOTFS_OUT_DIR/
fi

#
# 3. copy files to rootfs dir (make dir if not exists)
#
copy_files
if [ $? -ne 0 ]; then
	echo "err: copy_files failed!"
	dump_usage
	exit -1
else
	echo "copy_files ok!"
fi

#
# 4. execute extra cmd
#
exec_cmds
if [ $? -ne 0 ]; then
	echo "err: exec_cmds failed!"
	dump_usage
	exit -1
else
	echo "exec_cmds ok!"
fi

#
# 5. gen rootfs
#
gen_roofs
if [ $? -ne 0 ]; then
	echo "err: gen_roofs failed!"
	dump_usage
	exit -1
else
	echo "gen_roofs ok!"
fi

echo -e "\033[;31mGenerate rootfs success!\033[0m"

exit 0
