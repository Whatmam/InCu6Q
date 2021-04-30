#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=$PWD/../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf
if [ ! -d buildroot-2020.02.10 ]; then
         echo "Not Find buildroot Directory ... !"
         echo "Extract buildroot...."
	tar zxvf buildroot-2020.02.10.tar.gz
fi
cp -vrfp buildroot_configs/imx6_incu6_defconfig buildroot-2020.02.10/configs/
echo "Compile Start!!"
cd buildroot-2020.02.10

make clean
make imx6_incu6_defconfig
make all
