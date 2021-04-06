#!/bin/bash
result=0
echo "===================================================="
echo "          Check Total Uart Loopback Test!           "
echo "===================================================="
./uart_test 1 115200
result=$((result + $?))
./uart_test 3 115200
result=$((result + $?))
./uart_test 4 115200
result=$((result + $?))

if [ $result -eq 0 ]; then
        echo "===================================================="
        echo "       [Success] UART Loopback Total Test !!        "
        echo "===================================================="
        exit 0
else
        echo "===================================================="
        echo "       [Failed] UART Loopback Total Test !!         "
        echo "===================================================="
        exit -1
fi
