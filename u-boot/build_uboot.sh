#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-

make clean;make distclean;
make mx6dlsabresd_defconfig 
make all -j12
