#!/bin/sh


export ARCH=arm
export CROSS_COMPILE='../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-'

make imx_JW_defconfig
make zImage -j9
make dtbs

cp -vrfp arch/arm/boot/zImage ./zImage
cp -vrfp arch/arm/boot/dts/imx6dl-JW-incu6.dtb ./imx6dl-JW-rgb.dtb
