#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=../Toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

make clean;make distclean;
make mx6dlsabresd_defconfig 
make all -j12
