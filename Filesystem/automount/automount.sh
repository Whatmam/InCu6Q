#!/bin/sh
echo " Executing mount /dev/$1 to /media/$1, action=$ACTION" > /dev/console
if [ ${1} != "" ]
then
        if [ -e /media/ -a -e /media/${1} ]
        then
                umount /media/${1}
                umount /dev/${1}
                rmdir /media/${1}
        else
                mkdir -p /media/${1}
                mount /dev/${1} /media/${1}
        fi
else
        echo "mount ${1} error"
fi
:<<END
echo " Executing mount /dev/$1 to /media/$1, action=$ACTION" > /dev/console

if [ ${1} != "" ]
then
        if [ "$ACTION" == "remove" ]
        then
                umount /media/${1}
                umount /dev/${1}
                rmdir /media/${1}
        else if[ "$ACTION" == "add" ]
        echo "@@@TEST SHELL SCRIPT @@@" > /dev/console
                mkdir -p /media/${1}
                mount /dev/${1} /media/${1}
        fi
else
        echo "mount ${1} error"
fi
END

