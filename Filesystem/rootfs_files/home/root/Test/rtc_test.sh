#!/bin/bash
ETH_EXIST=`find /sys/class/net -name eth0`
ETH_LINK=`cat /sys/class/net/eth0/carrier`
result=0;

if [ $ETH_EXIST ] && [ $ETH_LINK -eq 1 ]; then
	echo "===================================================="
	echo "          [Ready] Ethernet PHY & Link Check OK."
	echo "===================================================="
	killall dhcpcd
	dhcpcd eth0
	sleep 7 
	rdate -s time.bora.net
	result=$((result + $?))
	date
	result=$((result + $?))
	hwclock -w -l
	result=$((result + $?))
	hwclock
	result=$((result + $?))
else
	echo "===================================================="
	echo "          [Ready] Ethernet PHY & Link Doesn't Exist."
	echo "                  Start Local Test!!"
	echo "===================================================="
	date 010101012020 
	result=$((result + $?))
	date
	result=$((result + $?))
	hwclock -w -l
	result=$((result + $?))
	hwclock
	result=$((result + $?))
fi

if [[ $result -eq 0 ]]; then
	echo "===================================================="
	echo "          [Success] RTC Setting Test OK !!"
	echo "                    Please Reboot(Btn) & Check      " 
	echo "===================================================="
	exit 0
else
	echo "===================================================="
	echo "          [Failed] RTC Setting Test Fail !"
	echo "===================================================="
	exit -1
fi
