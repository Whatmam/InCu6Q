#!/bin/sh
cd bl_ctrl
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

cd ..


#sd_test.sh
#cd uart_test
#usb_test.sh
#cd ethernet_test
cp gst_video_fb.sh ../Filesystem/rootfs_files/home/root/Test/
cp lcd_test.sh ../Filesystem/rootfs_files/home/root/Test/
#cp rtc_test.sh ../Filesystem/rootfs_files/home/root/Test/
