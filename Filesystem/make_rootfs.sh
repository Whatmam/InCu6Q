#!/bin/bash

export rootfs_base_image=rootfs_base.tar.bz2
export output=rootfs_output

#Delete Filesystem base image

if [ ! -e $rootfs_base_image ]; then
        echo "No search ${rootfs_base_image} ... !!!"
else

        if [ ! -e $output ]; then
                mkdir -p $output
        fi

        echo "Extract ${rootfs_base_image} ... !!!"
        tar -jxvf $rootfs_base_image -C $output
        echo "...Done"
        cd $output

        rm rootfs.tar.bz2

   # Copy files
        cp -rfvp ../rootfs_files/* .
	chown root:root -hR *
        tar -cjvf rootfs.tar.bz2 *
        find . ! -name rootfs.tar.bz2 -delete

   # Copy to MFGTOOL
        cp -rfvp rootfs.tar.bz2 ../../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/
        rm rootfs.tar.bz2
fi