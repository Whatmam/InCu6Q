#!/bin/sh


make imx_v7_defconfig
make zImage -j9
make dtbs

cp -vrfp arch/arm/boot/zImage ./zImage
cp -vrfp arch/arm/boot/dts/imx6dl-JW-incu6.dtb ./imx6dl-JW-rgb.dtb
