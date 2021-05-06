#!/bin/sh
export rootfs_base_image=rootfs_base.tar.bz2
export output=rootfs_output

echo "Extract ${rootfs_base_image} ... !!!"
tar -jxvf $rootfs_base_image -C $output
echo "...Done"
cd $output
cp -rfvp ../rootfs_files/* .
chown root:root -hR *
tar -cjvf rootfs.tar.bz2 *
# Copy files
cp -vrfp rootfs.tar.bz2 ../
find . ! -name rootfs.tar.bz2 -delete
rm rootfs.tar.bz2