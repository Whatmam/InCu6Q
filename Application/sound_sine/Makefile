CROSS_COMPILE = ../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC      = $(CROSS_COMPILE)gcc
TEST_APP = sine_test

sine_test: sine_test.o wavfile.o
	$(CC) -o sine_test wavfile.o sine_test.o -lm
	chmod 777 $(TEST_APP)


wavfile.o: wavfile.c wavfile.h
	$(CC) -c -o wavfile.o wavfile.c

sine_test.o: sine_test.c
	$(CC) -c -o sine_test.o sine_test.c -lm
clean:
	rm -rf *.o
	rm -rf $(TEST_APP)
