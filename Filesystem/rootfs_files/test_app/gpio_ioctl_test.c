#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>


#define             IOCTL_MAGIC         200
#define             SET_DATA            _IOW(IOCTL_MAGIC, 2 ,struct ioctl_info)
#define             GET_DATA            _IOR(IOCTL_MAGIC, 3 ,struct ioctl_info)

struct gpio_data
{
        unsigned int number; 
        unsigned int value;
};

struct ioctl_info{
       unsigned long size;
       char buf[128];
};

static int fd = 0;
static struct gpio_data gpio_data;
static struct ioctl_info set_info;
static struct ioctl_info get_info;


int func(int number, int value)
{
	if(fd == 0)
	{
		fd = open("/dev/gpio_switch", O_RDWR);
		if(fd < 0)
		{
			printf("[Error] Open gpio_switch Error.\n");
		}
	}

    gpio_data.number = number;
    gpio_data.value = value;

	// set_info.size = 123;
    // strncpy(set_info.buf,"CHS_GOOD",8);

	// if (ioctl(fd, SET_DATA, &set_info) < 0){
    //     printf("Error : SET_DATA.\n");
    // }
 
    // if (ioctl(fd, GET_DATA, &get_info) < 0){
    //     printf("Error : GET_DATA.\n");
    // }

	// printf("[APP]get_info.size : %ld, get_info.buf : %s\n", get_info.size, get_info.buf);


	//write(fd, (char *)&gpio_data, sizeof(gpio_data));
	ioctl(fd, gpio_data.number, gpio_data.value);


		if (close(fd) != 0){
        printf("Cannot close.\n");
    }
}

int main(int argc, char * argv[])
{
    int number, value;
	fd = open("/dev/gpio_switch", O_RDWR);
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
