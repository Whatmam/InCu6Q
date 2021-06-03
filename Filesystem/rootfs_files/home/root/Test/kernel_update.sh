#!/bin/sh
echo "Kernel Update Start!!"
target_dir=/mnt/mmcblk0p1
mkdir -p $target_dir
echo "Mount Boot Partition!!"
mount /dev/mmcblk0p1 $target_dir
mkdir -p $target_dir/backup
cp -vrfp $target_dir/*.dtb $target_dir/backup
cp -vrfp $target_dir/zImage $target_dir/backup
echo "Create Backup Files & Mount Done."

echo "Copying Update Files"

#BootLoader Update
if [ -e ./u-boot-dtb.imx ]; then
	dd if=./u-boot-dtb.imx of=/dev/mmcblk0 bs=512 seek=2 conv=fsync
    echo "Bootloader Update Finish"
else
    echo "Bootloader Doesn't Update"
fi

#Kernel Update
if [ -e ./imx6q-JW-incu6-lvds.dtb ] && [ -e ./imx6q-JW-incu6-rgb.dtb ] \
   && [ -e ./zImage ]; then
    cp -vrfp ./imx6q-JW-incu6-lvds.dtb $target_dir
    cp -vrfp ./imx6q-JW-incu6-rgb.dtb $target_dir
    cp -vrfp ./zImage $target_dir
    echo "Done"
else
    echo "Kernel Files Don't Exist"
    echo "Delete Backup Files & Umount Partition"
    rm -rf $target_dir/backup
    umount $target_dir
    rm -rf $target_dir
    echo "Update Failed !!!!!"
    exit 0
fi


echo "Delete Backup Files & Umount Partition"
rm -rf $target_dir/backup
umount $target_dir
rm -rf $target_dir
echo "Update Finish !!!!!"
