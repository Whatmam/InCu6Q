CROSS_COMPILE = ../../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC	= $(CROSS_COMPILE)gcc
GPIO_DRIVER_APP = gpio_driver_test
GPIO_SYS_APP = gpio_sysfs_test

all:
	$(CC) $(GPIO_DRIVER_APP).c -o $(GPIO_DRIVER_APP)
	chmod 777 $(GPIO_DRIVER_APP)
	$(CC) $(GPIO_SYS_APP).c -o $(GPIO_SYS_APP)
	chmod 777 $(GPIO_SYS_APP)

clean:
	rm -rf *.o
	rm -rf $(GPIO_DRIVER_APP)
	rm -rf $(GPIO_SYS_APP)

