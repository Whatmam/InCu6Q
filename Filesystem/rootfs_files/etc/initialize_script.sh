#!/bin/sh

lcd_cmd=`cat /proc/cmdline | grep lcd`
#echo $lcd_cmd
if [ -z "$lcd_cmd" ]; then
	cp -rfp /etc/10inch_pointcal /etc/pointercal
else
	cp -rfp /etc/7inch_pointcal /etc/pointercal
fi

#hw clock -> system clock
hwclock -s
