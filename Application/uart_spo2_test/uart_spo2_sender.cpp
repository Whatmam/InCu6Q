#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

enum pattern {
    TYPE1=0,
    TYPE2,
    TYPE3,
    TYPE4,
};

static char type1_buf[] = { 0x55, 0x02, 0x04, 0x7E, 0x02, 0x07, 0x9C, 0x2E, 0x03 };
static char type2_buf[] = { 0x55, 0x02, 0x12, 0x7E, 0x02, 0x07, 0x9B, 0x21, 0x04,
                            0x4B, 0x3C, 0x80, 0x10, 0x6A, 0x06, 0xC8, 0x4B, 0x80,
                            0x3C, 0x40, 0x16, 0x44, 0x03};
static char type3_buf[] = { 0x55, 0x02, 0x0a, 0x7e, 0x02, 0x07, 0x9c, 0x21, 0x04,
                            0x4b, 0x3c, 0x80, 0x10, 0x98, 0x03, };
static char type4_buf[] = { 0x55, 0x02, 0x0c, 0x7e, 0x02, 0x07, 0x9c, 0x6a, 0x06,
                            0xc8, 0x4b, 0x80, 0x3c, 0x40, 0x16, 0x12, 0x03 };

#define UART_DEVICE "/dev/ttymxc"
#define BAUDRATE B19200

int dev_open(char *dev, unsigned long baud_rate)
{
    struct termios oldtio, newtio;
    int dev_fd;
    unsigned int baud = 0;

    dev_fd = open(dev, O_RDWR | O_NOCTTY);
    if (dev_fd < 0)
    {
        perror(dev);
        exit(-1);
    }

    switch (baud_rate)
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

    tcgetattr(dev_fd, &oldtio);

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag = baud | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;

    newtio.c_oflag = 0;
    newtio.c_lflag = 0;

    newtio.c_cc[VMIN] = 0;
    newtio.c_cc[VTIME] = 10;

    tcflush(dev_fd, TCIFLUSH);
    tcsetattr(dev_fd, TCSANOW, &newtio);

    return dev_fd;
}

int main(int argc, char **argv)
{
    struct termios oldtio, newtio;
    char send_buf[20];
    char recv_buf[20];
    char opendev[20] = UART_DEVICE;
    int err_cnt = 0;
    int i = 0, uart_fd;
    unsigned long baud_rate;

    if (argv[1] == NULL || argc < 2)
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
    printf(" RX SpO2 Data Sender\n");
    printf("Device : %s\n", opendev);
    printf("====================================================\n");
    while (1)
    {
        switch (i % 3)
        {
        case TYPE1:
            write(uart_fd, type1_buf, sizeof(type1_buf));
            break;
        case TYPE2:
            write(uart_fd, type2_buf, sizeof(type2_buf));
            break;
        case TYPE3:
            write(uart_fd, type3_buf, sizeof(type3_buf));
            break;
        case TYPE4:
        default:
            write(uart_fd, type4_buf, sizeof(type4_buf));
            break;
        }
        i++;
        usleep(3000);
    }
    close(uart_fd);

    return 0;
}