#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=$PWD/../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf

make clean
make imx6_incu6_defconfig
make all -j12

# if [ ! -e rootfs_files ]; then
#         echo "Not Find rootfs_files ... !"
# else
# cd $PWD/rootfs_files
# 		if [ -e copy_files.sh ]; then
# 			./copy_files.sh 
# 		else
# 			echo "Not Find copy_files.sh ... !"
# 		fi
# fi
