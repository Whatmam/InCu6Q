#!/bin/sh
export KERNEL_SRC=$PWD
export INSTALL_MOD_PATH=../Filesystem/rootfs_files
if [ ! -e $INSTALL_MODE_PATH ]; then
	mkdir -p $INSTALL_MODE_PATH
fi
if [ -e ./zImage ] ;  then
	rm ./zImage
fi
rm ./*.dtb
make imx_JW_defconfig
make zImage -j9
make dtbs
make modules
make modules_install
cp -vrfp arch/arm/boot/zImage ./zImage
cp -vrfp zImage ../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/
cp -vrfp arch/arm/boot/dts/imx6dl-JW-incu6.dtb ./imx6dl-JW-rgb.dtb
cp -vrfp arch/arm/boot/dts/imx6q-JW-incu6.dtb ./imx6q-JW-rgb.dtb
cp -vrfp *.dtb ../i.MX6_MFG_Tools_JW/Profiles/Linux/OS\ Firmware/files/

export ARCH=arm
export CROSS_COMPILE='../Toolchain/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-'
#cd kernel-module-imx-gpu-viv
#make
#make modules_install
