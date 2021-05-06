#!/bin/sh
device_name=$1

umount "/media/$device_name"
#umount "/dev/$device_name"
rmdir "/media/$device_name"