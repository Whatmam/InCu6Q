#!/bin/bash

export rootfs_build_image=../buildroot/output/images/rootfs.tar.bz2
export rootfs_base_image=rootfs_files/rootfs_base/rootfs.tar.bz2
export output=rootfs_output

   #Delete Filesystem base image

if [ "$1" == clean ]; then
        rm -v $rootfs_base_image
        exit
fi


   #Copy build buildroot image

if [ "$1" == copy ]; then
        if [ ! -e $rootfs_build_image ]; then
                echo "No search ${rootfs_build_image} ... !!!"
        else
                cp -rfvpi ${rootfs_build_image} ${rootfs_base_image}
        fi
        exit
fi


if [ ! -e $rootfs_base_image ]; then
        echo "No search ${rootfs_base_image} ... !!!"
else

        if [ ! -e $output ]; then
                mkdir -p $output
        fi

        cp -vrfp $rootfs_base_image $output
        cd $output

        find . ! -name rootfs.tar.bz2 -delete
        tar -xvf rootfs.tar.bz2
        rm rootfs.tar.bz2

   # Copy files
        cp -rfvp ../rootfs_files/etc .
        cp -rfvp ../rootfs_files/lib .
        cp -rfvp ../rootfs_files/sbin .
        cp -rfvp ../rootfs_files/root .


        tar -cjvf rootfs.tar.bz2 *
        find . ! -name rootfs.tar.bz2 -delete

   # Copy to MFGTOOL
        cp -rfvp rootfs.tar.bz2 ../../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/
fi