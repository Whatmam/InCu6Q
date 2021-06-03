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

#define             SET_LED_CTL_1       _IOW(IOCTL_MAGIC, 110 ,struct ioctl_info)
#define             SET_LED_CTL_2       _IOW(IOCTL_MAGIC, 111 ,struct ioctl_info)
#define             SET_LED_CTL_3       _IOW(IOCTL_MAGIC, 116 ,struct ioctl_info)

#define             LED_CTL_1            110     //GPIO4_14
#define             LED_CTL_2            111     //GPIO4_15
#define             LED_CTL_3            116     //GPIO4_20

struct gpio_data
{
        unsigned int number; 
        unsigned int value;
};

struct ioctl_info{
        unsigned int number; 
        unsigned int value;
};

static int fd = 0;
static struct gpio_data gpio_data;
static struct ioctl_info set_info;
static struct ioctl_info get_info;

void print_help()
{
		printf("Usage : gpio_test [option]\n");
		printf("Options:\n");
		printf("\t-v \t\t\t\tShow GPIO Pin map\n");
		printf("\t-r [sys number] \t\tGPIO Read\n");
		printf("\t-w [sys number] [value] \tGPIO Wirte\n");
		printf("\t-h \t\t\t\tthis help\n");
}
int gpio_read(int number)
{
if(fd == 0)
	{
		fd = open("/dev/gpio_jw", O_RDWR);
		if(fd < 0)
		{
			printf("[Error] Open gpio_jw Error.\n");
		}
	}

    gpio_data.number = number;
	gpio_data.value = 1;

	ioctl(fd, gpio_data.number, gpio_data.value);

	
    if (ioctl(fd, GET_DATA, &get_info) < 0){
        printf("Error : GET_DATA.\n");
    }

	printf("[READ] Number : %d, value : %d\n", get_info.number, get_info.value);


	if (close(fd) != 0){
        printf("Cannot close.\n");
    }


}
int gpio_write(int number, int value)
{
	if(fd == 0)
	{
		fd = open("/dev/gpio_jw", O_RDWR);
		if(fd < 0)
		{
			printf("[Error] Open gpio_jw Error.\n");
		}
	}

    gpio_data.number = number;
    gpio_data.value = value;

	gpio_data.value |= 0x1 <<1;
	// value : 2 -> low 3 high
	if(gpio_data.number == LED_CTL_1)
	{
		set_info.number = LED_CTL_1;
		set_info.value = value;
		ioctl(fd, SET_LED_CTL_1, &set_info);
	}
	else if(gpio_data.number == LED_CTL_2)
	{
		set_info.number = LED_CTL_2;
		set_info.value = value;
		ioctl(fd, SET_LED_CTL_2, &set_info);
	}
	else if(gpio_data.number == LED_CTL_3)
	{
		set_info.number = LED_CTL_3;
		set_info.value = value;
		ioctl(fd, SET_LED_CTL_3, &set_info);
	}
	else
	{
		ioctl(fd, gpio_data.number, gpio_data.value);
	}

	if (close(fd) != 0){
        printf("Cannot close.\n");
    }
}

int main(int argc, char * argv[])
{
	int gpio_number, value;


	fd = open("/dev/gpio_jw", O_RDWR);
	if(argc < 2)
	{
		print_help();
		return 0;
	}

	if(!strcmp(argv[1], "-v"))
	{

		printf ("+-----+---GPIO-----------+----JC7---+\n"
				"| sys |   Name           | Physical |\n"
				"+-----+------------------+----------+\n"
				"|  80 | GPIO3_16         |     1    |\n"
				"|  81 | GPIO3_17         |     2    |\n"
				"|  82 | GPIO3_18         |     3    |\n"
				"|  83 | GPIO3_19         |     4    |\n"
				"|  84 | GPIO3_20         |     5    |\n"
				"| 160 | GPIO6_00         |     6    |\n"
				"| 161 | GPIO6_01         |     7    |\n"
				"| 162 | GPIO6_02         |     8    |\n"
				"| 163 | GPIO6_03         |     9    |\n"
				"|     | DGND             |    10    |\n"		
				"+-----+------------------+----U21---+\n"
				"| 200 | GPIO_V50_Out_CTL |    22    |\n"	
				"+-----+------------------+----U22---+\n"
				"| 204 | GPIO_V50_In_CTL  |    22    |\n"	
				"+-----+------------------+----------+\n\n");									

		printf ("+-----+---AD Convertor---+----JC6---+\n"
				"| sys |       Name       | Physical |\n"
				"+-----+------------------+----------+\n"
				"|     | VDD_V50D         |     1    |\n"
				"| 106 | GPIO4_10         |     2    |\n"
				"|     | DGND             |     3    |\n"
				"+-----+------------------+----------+\n\n");

		printf ("+-----+---Alarm LED CTL--+----JC4---+\n"
				"| sys |       Name       | Physical |\n"
				"+-----+------------------+----------+\n"
				"| 110 | Alarm_RED_CTL    |     1    |\n"
				"| 111 | Alarm_Yellow_CTL |     2    |\n"
				"| 116 | Alarm_GREEN_CTL  |     3    |\n"
				"|     | VDD_V50D         |     4    |\n"
				"|     | VDD_V50D         |     5    |\n"
				"|     | DGND             |     6    |\n"
				"|     | DGND             |     7    |\n"
				"+-----+------------------+----------+\n\n");

		printf ("+-----+-MainPower-+----JS1---+\n"
				"| sys |   Name    | Physical |\n"
				"+-----+-----------+----------+\n"
				"|     | GND       |     1    |\n"
				"|     | GND       |     2    |\n"
				"|     | GND       |     3    |\n"
				"|     | UART3_TX  |     4    |\n"
				"|     | UART3_RX  |     5    |\n"
				"|     | UART2_TX  |     6    |\n"
				"|     | UART2_RX  |     7    |\n"
				"|     | UART1_TX  |     8    |\n"
				"|     | UART1_RX  |     9    |\n"
				"|     | Photo_Sen |    10    |\n"
				"|     | UART0_TX  |    11    |\n"
				"|     | UART0_RX  |    12    |\n"
				"| 152 | GPIO_Out0 |    13    |\n"
				"| 153 | GPIO_Out1 |    14    |\n"
				"| 154 | GPIO_Out2 |    15    |\n"
				"| 155 | GPIO_Out3 |    16    |\n"
				"| 148 | GPIO_In0  |    17    |\n"
				"| 149 | GPIO_In1  |    18    |\n"
				"| 150 | GPIO_In2  |    19    |\n"
				"| 151 | GPIO_In3  |    20    |\n"
				"|     | BAT_LED_R |    21    |\n"
				"|     | BAT_LED_G |    22    |\n"
				"|     | GND       |    23    |\n"
				"|     | 5V        |    24    |\n"
				"|     | 5V        |    25    |\n"
				"|     | 12V       |    26    |\n"
				"+-----+-----------+----------+\n\n");

		return 0;
	}
	else
	{
	
	if(!strcmp(argv[1], "-w"))
	{
		if (argc < 4)
		{
			print_help();
			return 0;
		}

		gpio_number = atoi(argv[2]);
		value = atoi(argv[3])> 0 ? 1 : 0;
		
		if(gpio_number <0 || gpio_number >256 )
		{
			printf("Invalid Parameter \n");
			return 0;
		}
		gpio_write(gpio_number,value);
	}
	else if(!strcmp(argv[1], "-r"))
	{
		if (argc < 3)
		{
			print_help();
			return 0;
		}

		gpio_number = atoi(argv[2]);

		if(gpio_number <0 || gpio_number >256)
		{
			printf("Invalid Parameter \n");
			return 0;
		}
		gpio_read(gpio_number);
	}
	else if(!strcmp(argv[1], "-h"))
	{
		print_help();
	}
	else
	{
		print_help();
	}

	}
	close(fd);
	return 0;
}
