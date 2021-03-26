#!/bin/bash

   # Create buildroot
cd ..
tar -xvf Filesystem/buildroot-2020.02.10.tar.gz
mv buildroot-2020.02.10 buildroot

   # Copy defconfig
cp -rfvp Filesystem/buildroot_configs/imx6_incu6_defconfig buildroot/configs/

   # QT5.12.3 configure
cp -rfvp Filesystem/QT5_Config/package/ buildroot/

   # Copy build Script
cp -rfvp Filesystem/build_Filesystem.sh buildroot/