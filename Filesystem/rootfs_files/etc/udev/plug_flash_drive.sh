#!/bin/sh

device_name=$1
mount_options="umask=000,utf8"
if [ ! -e "/media/$device_name" ]; then
    mkdir "/media/$device_name"
fi
sleep 1
mount "/dev/$device_name" "/media/$device_name" -o "$mount_options"