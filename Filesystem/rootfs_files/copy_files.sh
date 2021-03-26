#!/bin/sh

export rootfs_image_path=../output/images/rootfs.tar.bz2
export output=rootfs_output


if [ ! -e $rootfs_image_path ]; then
	echo "Not Find rootfs.tat.bz2 ... !"
else

	
if [ ! -e $output ]; then
        mkdir -p $output
fi

cp -vrfp $rootfs_image_path $output
cd $output

find . ! -name rootfs.tar.bz2 -delete
tar -xvf rootfs.tar.bz2
rm rootfs.tar.bz2

# COPY FILES
cp -rfvp ../etc .
cp -rfvp ../lib .
cp -rfvp ../sbin .
cp -rfvp ../root .


tar -cjvf rootfs.tar.bz2 *
find . ! -name rootfs.tar.bz2 -delete
fi
