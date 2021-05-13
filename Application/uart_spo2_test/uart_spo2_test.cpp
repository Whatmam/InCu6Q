#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <string.h>

#include <pthread.h>

#include <iostream>
#include <queue>
using namespace std;

int uart_fd;

queue<char> serial_queue;
bool read_thread_run = false;
bool parsing_thread_run = false;
#define STH	0x55
#define STX 0x02
#define ETX 0x03

void *t_serial_func(void * data)
{
	int rd_size = 0;
	int i = 0;
	char buf[100];
	//pid_t pid;
	//pthread_t tid;


	printf("thread start 1 !!\n");
	while(read_thread_run)
	{
		if(uart_fd < 0)
		{
			printf("serial port Error\n");
			break;
		}
		rd_size = read(uart_fd, buf, sizeof(buf));
		if(rd_size < 0)
		{
			printf("read null\n");
			continue;
		}
		for(i = 0; i<rd_size; i++)	
		{
			serial_queue.push(buf[i]);
		}
	}
}

void *t_parsing_func(void* data)
{
	char parsed_packet[100];
	int packet_length = 0;
	unsigned long parsed_data = 0, packet_count = 0;
	bool packet_start = false;

	printf("thread start 2 !!\n");
	while(parsing_thread_run)
	{
		while(!serial_queue.empty())
		{
			char tmp_data = serial_queue.front();
			serial_queue.pop();
			if(tmp_data == STH)
			{
				packet_length = 0;
				parsed_packet[0] = tmp_data;
				packet_length++;
				packet_start = true;
			}	
			else if(tmp_data == ETX)
			{
				parsed_packet[packet_length] = tmp_data;
				printf("Parsed Packet : ");
				for(int i =0; i <= packet_length; i++)
				{
					printf("0x%x ", parsed_packet[i]);
				}
				printf(" -> size = %d\n", packet_length);
				packet_start = false;
				parsed_data += packet_length + 1;
				printf("packet Count : %d => Total Parsed Bytes : %d\n", packet_count++, parsed_data);
			}
			else
			{
				if(packet_start)
				{
					if(packet_length > 3)
					{
						if(parsed_packet[2] + 5 < packet_length)
						{
							printf("Data Corruption : size_ packet(%d) < packet_length(%d + 2(CRC + ETX))\n", parsed_packet[2], packet_length + 2);
							packet_start = false;
							packet_length = 0;
							continue;
						}
					}
					parsed_packet[packet_length++] = tmp_data;
				}
				else
					printf("Unknown Packet data : 0x%x\n", tmp_data);	
			}
		}
	}
}

#define UART_DEVICE	"/dev/ttymxc"
#define	BAUDRATE	B19200

int dev_open(char *dev, unsigned long baud_rate){
        struct termios oldtio,newtio;
        int dev_fd;
	unsigned int baud=0;

        dev_fd = open(dev, O_RDWR | O_NOCTTY );
        if (dev_fd <0) {perror(dev); exit(-1); }

	switch(baud_rate)
	{
		case 9600:
		      baud = B9600;
		      break;
		case 19200: 
		      baud = B19200;
		      break;
		case 38400:
		      baud = B38400;
		      break;
		case 115200:
		      baud = B115200;
		      break;
		case 1000000:
		      baud = B1000000;
		      break;
		case 4000000:
		      baud = B4000000;
		      break;
		default:
		      printf("unknown Baudrate : %d\n", baud_rate);
		      return -1;
	}

        tcgetattr(dev_fd,&oldtio);

        memset(&newtio, 0, sizeof(newtio));
        newtio.c_cflag = baud | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;

        newtio.c_oflag = 0;
        newtio.c_lflag = 0;

        newtio.c_cc[VMIN]     = 0;
        newtio.c_cc[VTIME]    = 10;

        tcflush(dev_fd, TCIFLUSH);
        tcsetattr(dev_fd,TCSANOW,&newtio);

        return dev_fd;
}

int main(int argc, char ** argv){
	struct termios oldtio,newtio;
	char send_buf[20];
	char recv_buf[20];
	char opendev[20] = UART_DEVICE;
	int err_cnt = 0;
	int i = 0;
	unsigned long baud_rate;

	pthread_t p_thread[2];
	int thr_id, status;

	if(argv[1] == NULL || argc < 2)
	{
		printf("./uart_spo2_test [uart_num]\n");
		printf("Please input ttymxc number\n");
		printf("ex) 1 2 3 4\n");
		return -1;
	}
	strcat(opendev, argv[1]);
	baud_rate = 19200;

	uart_fd = dev_open(opendev, baud_rate);
	if (uart_fd < 0)
	{
		perror(opendev);
		exit(-1);
	}

	printf("====================================================\n");
	printf("[Ready] Serial Driver Setting\n");
	printf(" RX SpO2 Data Read Test\n");
	printf("Device : %s\n", opendev);
	printf("====================================================\n");
	thr_id = pthread_create(&p_thread[0], NULL, t_serial_func, (void *)NULL);

	read_thread_run = true;

	if(thr_id < 0)
	{
		perror("thread create error\n");
		exit(-1);
	}

	parsing_thread_run = true;
	thr_id = pthread_create(&p_thread[1], NULL, t_parsing_func, (void *)NULL);

	if(thr_id < 0)
	{
		perror("thread create error\n");
		exit(-1);
	}
	
	pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);

	if(err_cnt == 0)
	{
		printf("====================================================\n");
                printf("         [Success] Serial LoopBack Test !!\n");
		printf("                   Device : %s\n", opendev);
                printf("====================================================\n");
		return 0;
	}
	else
	{
		printf("====================================================\n");
                printf("         [Failed] Serial LoopBack Test !!\n");
		printf("                  Device : %s\n", opendev);
                printf("====================================================\n");
		return -1;
	}
	
}
