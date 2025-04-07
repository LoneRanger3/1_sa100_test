#!/bin/bash

if [ ! -d "tmpmnt" ];then
	mkdir tmpmnt
fi

dd if=/dev/zero of=fsdata.img bs=1k count=1k

/sbin/mkfs.fat fsdata.img

sudo mount -t vfat -o umask=0000 fsdata.img tmpmnt/
cp -rf files/bootfs_raw/* tmpmnt/
sudo umount tmpmnt/

rm -fr tmpmnt
mv fsdata.img files/out/bootfs.img

echo -e "\033[;31mGenerate bootfs success!\033[0m"

exit 0