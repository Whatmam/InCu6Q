#!/bin/sh
cd adc_test
make
cp -vrfp  adc_test ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../bl_ctrl
make
cp -vrfp  bl_ctrl ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../gpio_test
make
cp -vrfp  gpio_test ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../uart_test
make
cp -vrfp  uart_test ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../mk_raw_image
make
cp -vrfp mk_raw_image ../../Filesystem/rootfs_files/home/root/Test/
cp -vrfp fb_test ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../v4l2-framebuffer
make
cp -vrfp  v4l2-framebuffer ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ../ethernet_test
make
cp -vrfp  ethernet_test ../../Filesystem/rootfs_files/home/root/Test/
make clean

cd ..

cp sd_test.sh ../Filesystem/rootfs_files/home/root/Test/
cp usb_test.sh ../Filesystem/rootfs_files/home/root/Test/
cp uart_total.sh ../Filesystem/rootfs_files/home/root/Test/
cp gst_video_fb.sh ../Filesystem/rootfs_files/home/root/Test/
cp lcd_test.sh ../Filesystem/rootfs_files/home/root/Test/
cp rtc_test.sh ../Filesystem/rootfs_files/home/root/Test/
cp total_test/test_dialog ../Filesystem/rootfs_files/home/root/Test/
