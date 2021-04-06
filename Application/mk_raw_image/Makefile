CROSS_COMPILE = ../../Toolchain/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC      = $(CROSS_COMPILE)gcc
MK_ROW_APP = mk_raw_image
FB_APP = fb_test

all:
	$(CC) $(MK_ROW_APP).c -o $(MK_ROW_APP)
	chmod 777 $(MK_ROW_APP)
	$(CC) $(FB_APP).c -o $(FB_APP)
	chmod 777 $(FB_APP)

clean:
	rm -rf *.o
	rm -rf $(MK_ROW_APP)
	rm -rf $(FB_APP)
