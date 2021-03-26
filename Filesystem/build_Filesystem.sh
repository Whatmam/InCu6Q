#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=$PWD/../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf

make clean
make imx6_incu6_defconfig
make all -j12