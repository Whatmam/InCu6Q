CROSS_COMPILE = ../../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC      = $(CROSS_COMPILE)g++
TEST_APP = uart_spo2_test
TEST_APP_SENDER = uart_spo2_sender

all:
	$(CC) $(TEST_APP).cpp -o $(TEST_APP) -lpthread
	$(CC) $(TEST_APP_SENDER).cpp -o $(TEST_APP_SENDER)
	chmod 777 $(TEST_APP)

clean:
	rm -rf *.o
	rm -rf $(TEST_APP)
	rm -rf $(TEST_APP_SENDER)
