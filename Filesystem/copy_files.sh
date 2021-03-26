#!/bin/sh

export rootfs_image_base=../buildroot/output/images/rootfs.tar.bz2
export rootfs_image_path=rootfs_files/rootfs_base/rootfs.tar.bz2
export output=rootfs_output


if [ ! -e $rootfs_image_base ]; then
	echo "Not Find rootfs.tat.bz2 ... !"
else        
        cp -vrfp ../buildroot/output/images/rootfs.tar.bz2 rootfs_files/rootfs_base/
fi

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
cp -rfvp ../rootfs_files/etc .
cp -rfvp ../rootfs_files/lib .
cp -rfvp ../rootfs_files/sbin .
cp -rfvp ../rootfs_files/root .


tar -cjvf rootfs.tar.bz2 *
find . ! -name rootfs.tar.bz2 -delete
fi

# COPY TO MFGTOOL
cp -rfvp rootfs.tar.bz2 ../../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/