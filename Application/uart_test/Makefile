CROSS_COMPILE = ../../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC      = $(CROSS_COMPILE)gcc
TEST_APP = uart_test

all:
	$(CC) $(TEST_APP).c -o $(TEST_APP)
	chmod 777 $(TEST_APP)

clean:
	rm -rf *.o
	rm -rf $(TEST_APP)
