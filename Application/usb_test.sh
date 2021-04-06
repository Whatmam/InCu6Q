#!/bin/bash

echo "===================================================="
echo "            Check Mounted USB Memory!!"
echo "===================================================="

function success() {
        echo "===================================================="
        echo "          [Success] USB Mount CheckTest !!"
        echo "===================================================="
        exit 0
}

function fail() {
        echo "===================================================="
        echo "          [Failed] USB Mount CheckTest !!"
        echo "===================================================="
        exit -1
}

sleep 2;
echo "....."


DF_VALUE=`df | grep '/mnt/sd*'`
DIR_VALUE=`ls /mnt | grep 'sd*'`

echo "$DF_VALUE"
echo -e "mnt directory list : \n""$DIR_VALUE"
string_index=${#DF_VALUE}
string_index=$((${#DF_VALUE} - 4))
DIR_VALUE=${DF_VALUE:($string_index):4}
echo "Testing Dir :""$DIR_VALUE"
speed_per=0

if [ "$DF_VALUE" -a "$DIR_VALUE" ]; then

        wr_speed=$(dd if=/dev/zero of=/mnt/"$DIR_VALUE"/test bs=1M count=10 conv=fsync 2>&1)
        echo $wr_speed
        var=$(echo ${wr_speed##*,})
        echo "Write Speed : ""${var}"

        if [[ $var == *"kB/s"* ]]; then
                speed_per=1
                wr_index=`expr index "$var" "kB/s"`
        else
                speed_per=0
                wr_index=`expr index "$var" "MB/s"`
        fi

        wr_index=$((wr_index - 2))
        wr_speed=${var:0:$wr_index}
        rm -rf /mnt/"$DIR_VALUE"/test


        rd_speed=$(hdparm -t /dev/"$DIR_VALUE")
        echo $rd_speed
        var=$(echo ${rd_speed##*=})
        echo "Read Speed : ""${var}"

        rd_index=`expr index "$var" "kB/s"`
        rd_index=$((rd_index - 2))
        rd_speed=${var:0:($rd_index)}



        if [ $speed_per -eq 0 ]; then
                if [[ `expr $wr_speed > 1.2` == "" ]] && [[ $rd_speed -ge 12000 ]]; then
                        rm -rf 1.2
                        success
                else
                        rm -rf $wr_speed
                        fail
                fi
        else
                if [[ $wr_speed -ge 300 ]] && [[ $rd_speed -ge 12000 ]]; then
                        success
                else
                        fail
                fi
        fi
else
        fail
fi
