#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-

make clean;make distclean;
make mx6_jw_incu6_defconfig
make all -j12
cp -vrfp u-boot-dtb.imx ../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/
