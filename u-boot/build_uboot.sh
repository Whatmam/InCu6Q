#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
make clean;make distclean;
make mx6_jw_incu6_defconfig
make all -j12
cp -vrfp u-boot-dtb.imx ../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/
