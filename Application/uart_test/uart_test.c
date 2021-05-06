#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <string.h>

#define UART_DEVICE	"/dev/ttymxc"
#define	BAUDRATE	B115200

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
	int uart_fd;
	int err_cnt = 0;
	int i = 0;
	unsigned long baud_rate;

	if(argv[1] == NULL || argc < 3)
	{
		printf("./uart_test [uart_num] [baud_rate]\n");
		printf("Please input ttymxc number\n");
		printf("ex) 1 2 3 4\n");
		return -1;
	}
	strcat(opendev, argv[1]);
	baud_rate = atol(argv[2]);

	uart_fd = dev_open(opendev, baud_rate);
	if (uart_fd < 0) {
                perror(opendev);
                exit(-1);
        }
        printf("====================================================\n");
        printf("[Ready] Serial Driver Setting\n");
	printf(" RX<=>TX Loopback Test\n");
	printf("Device : %s\n", opendev);
        printf("====================================================\n");
	sleep(1);

	while(i < 5){
		sprintf(send_buf, "abcdefghijklmnopqrs\0");

		write(uart_fd, send_buf, sizeof(send_buf));
		read(uart_fd, recv_buf, sizeof(recv_buf));
		if(!strcmp(send_buf, recv_buf))
		{
			printf("read : %s\n", recv_buf);
		}
		else
		{
			printf("read error : %s\n", recv_buf);
			err_cnt++;
			break;
		}
		i++;
		sleep(1);
	}
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
