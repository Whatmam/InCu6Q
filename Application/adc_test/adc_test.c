#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#define ADC_PATH_DEV	"/dev/iio:device0"
#define ADC_PATH        "/sys/bus/iio/devices/iio:device0/in_voltage_raw"

int main(int argc, char * argv[])
{
	int i, fd = 0; 
        FILE *fp;
        char value=0;
        char buffer[16];
        char time;
        int read_size, mode = 0;
        if(argc > 1)
        {
                if(strcmp(argv[1], "one_shot"))
                {
                        mode = 0;
                        printf("Start Polling One Shot Mode\n");
                }
                else if(strcmp(argv[1], "sys_buf"));
                {
                        mode = 1;
                        system("echo 0 > /sys/bus/iio/devices/iio:device0/buffer/enable");
                        system("mkdir /sys/kernel/config/iio/triggers/hrtimer/trigger0");
                        system("echo 10 > /sys/bus/iio/devices/trigger0/sampling_frequency");
                        system("echo trigger0 > /sys/bus/iio/devices/iio:device0/trigger/current_trigger");
                        system("echo 1 > /sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en");
                        system("echo 1 > /sys/bus/iio/devices/iio:device0/scan_elements/in_voltage_en");
                        system("echo 1 > /sys/bus/iio/devices/iio:device0/buffer/enable");
                        printf("Start Polling Sys hrtimer buffer Mode(Default)\n");
                }
        }
        else
        {
                mode = 0;
                printf("Start Polling One Shot Mode(Default)\n");
        }
        memset(buffer, 0, sizeof(buffer));

        while (1)
        {
                if (!mode)
                {
                        fp = fopen(ADC_PATH, "r");
                        if (!fp)
                        {
                                printf("[Error] Open ADC Driver Error.\n");
                                return -1;
                        }

                        //if(read(fd, &value, sizeof(value)) < 0)
                        if (fscanf(fp, "%d", &value) < 0)
                        {
                                printf("ADC Read Fail\n");
                        }
                        else
                        {
                                printf("ADC Value : %d\n", value);
                        }
                        sleep(1);
                }
                else
                {
                        fd = open(ADC_PATH_DEV, O_RDONLY | O_NONBLOCK);
                        if( fd <= 0 )
                        {
                                return -1;
                        }
                        read_size = read(fd, &buffer, 1);
                        if(read_size > 0)
                        {
                                printf("read_bytes : %d\n", read_size);
                                printf("iio :", buffer[i]);
                                for(i=0; i<read_size; i++)
                                {
                                        printf("%04x ", buffer[i]);
                                }
                                printf("\n");
                                value = (buffer[0] >> 4);
                                printf("ADC Value : %d\n", value);
                        }
                        else
                        {
                                printf("read_bytes ?: %d\n", read_size);
                        }
                        //sleep(1);
                }
        }
        if (fp > 0)
        {
                fclose(fp);
        }

        if(fd)
        {
                close(fd);
        }
        system("echo 0 > /sys/bus/iio/devices/iio:device0/buffer/enable");
        return 0;
}
