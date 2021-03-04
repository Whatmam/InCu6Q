#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>

static int fd = 0;

struct gpio_data
{
        unsigned int number; 
        unsigned int value;
};
static struct gpio_data gpio_data;


int func(int number, int value)
{
	if(fd == 0)
	{
		fd = open("/dev/gpio_keys", O_RDWR);
		if(fd < 0)
		{
			printf("[Error] Open gpio_keys Error.\n");
		}
	}
        gpio_data.number = number;
        gpio_data.value = value;

	write(fd, (char *)&gpio_data, sizeof(gpio_data));
}

int main(int argc, char * argv[])
{
    int number, value;
	fd = open("/dev/gpio_keys", O_RDWR);
	if(argc < 3)
	{
		printf("Usage : [GPIO number] [1 OR 0 (HIGH OR LOW)]\n");
		return 0;
	}

	number = atoi(argv[1]);
	value = atoi(argv[2]);

    func(number,value);
	close(fd);
	return 0;
}
