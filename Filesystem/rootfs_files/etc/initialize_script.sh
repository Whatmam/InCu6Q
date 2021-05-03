#!/bin/sh

lcd_cmd=`cat /proc/cmdline | grep lcd`
#echo $lcd_cmd
if [ -z "$lcd_cmd" ]; then
        cp -rfp /etc/10inch_pointcal /etc/pointcal
else
        cp -rfp /etc/8inch_pointcal /etc/pointcal
fi
