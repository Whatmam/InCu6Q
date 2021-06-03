#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUF 200
#define OUTPUT_MODE 1
#define INPUT_MODE 0

int gpio_write(int gpio_number, int value);
int gpio_read(int gpio_number);
void exportPin(int gpio_number);
void unexportPin(int gpio_number);
void setDirectPin(int gpio_number, int direct);
void print_help();


void print_help()
{
		printf("Usage : gpio_test [option]\n");
		printf("Options:\n");
		printf("\t-v \t\t\t\tShow GPIO Pin map\n");
		printf("\t-r [sys number] \t\tGPIO Read\n");
		printf("\t-w [sys number] [value] \tGPIO Wirte\n");
		printf("\t-h \t\t\t\tthis help\n");
}


int main(int argc, char * argv[])
{
	int fd, gpio_number, value;

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

		exportPin(gpio_number);
		setDirectPin(gpio_number, OUTPUT_MODE);
		printf(">> write %d pin : %d\n", gpio_number, value);
		gpio_write(gpio_number,value);
		unexportPin(gpio_number);
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
		exportPin(gpio_number);
		setDirectPin(gpio_number, INPUT_MODE);
		printf(">> read  %d pin : %d\n", gpio_number, gpio_read(gpio_number));
		unexportPin(gpio_number);
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
int gpio_write(int gpio_number, int value)
{
	int fd;
	char buf[MAX_BUF];
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio_number);

	fd = open(buf, O_WRONLY);

	if (value)
		write(fd, "1", 1);
	else
		write(fd, "0", 1);
	close(fd);
}

int gpio_read(int gpio_number)
{
	int fd;
	int readData = 0;
	char value;
	char buf[MAX_BUF];
	{
		sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio_number);

		fd = open(buf, O_RDONLY);

		read(fd, &value, 1);

		if (value == '0')
		{
			readData = 0;
		}
		else
		{
			readData = 1;
		}

		close(fd);
	}
	return readData;
}
void exportPin(int gpio_number)
{
	int fd;
	char buf[MAX_BUF];
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio_number);
	write(fd, buf, strlen(buf));
	close(fd);
}

void unexportPin(int gpio_number)
{
	int fd;
	char buf[MAX_BUF];
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio_number);
	write(fd, buf, strlen(buf));
	close(fd);
}

void setDirectPin(int gpio_number, int direct)
{
	int fd;
	char buf[MAX_BUF];
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio_number);
	fd = open(buf, O_WRONLY);
	if (direct)
	{
		write(fd, "out", 3);
	}
	else
	{
		write(fd, "in", 2);
	}
	close(fd);
}
