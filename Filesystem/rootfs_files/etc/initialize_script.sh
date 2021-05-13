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

#GPIO Control
echo 200 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio200/direction
echo 0 > /sys/class/gpio/gpio200/value

echo 204 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio204/direction
echo 0 > /sys/class/gpio/gpio204/value

