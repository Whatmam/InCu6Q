#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int ping_test(char * dest)
{
	FILE* fp;
	int retval, i=0;
	char path[1024];
	char result_string[10240];
	char cmd[1024];

	sprintf(cmd, "ping %s -c 5 -W 1", dest); 
	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed to run ping command\n");
	}
	while (fgets(path, sizeof(path)-1, fp) != NULL) {
		strcpy(&result_string[i], path);
		i += strlen(path);
		printf("%s", path);
	}
	pclose(fp);



	if(strstr(result_string, "5 packets transmitted, 5 received, 0% packet loss"))
	{
		printf("====================================================\n");
		printf("         [Success] Ethernet Ping 5c Test !!\n");
		printf("====================================================\n");
		return 0;
	}
	else
	{
		printf("====================================================\n");
		printf("         [Failed]  Ethernet Ping 5c Test !!\n");
		printf("====================================================\n");
		return -1;
	}
}

int iperf_test(char * dest)
{
	FILE* fp;
	int retval, i=0;
	char path[1024];
	char result_string[10240];
	char * result_str2;
	char cmd[1024];
	float send_throughput;

	sprintf(cmd, "iperf3 -c %s -t 10 --connect-timeout 3000", dest);

	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed to run ping command\n");
	}

	while (fgets(path, sizeof(path)-1, fp) != NULL) {
		strcpy(&result_string[i], path);
		i += strlen(path);
		printf("%s", path);
	}
	

	if(result_str2 !=NULL)
	{
		result_str2 = strstr(result_string, "0.00-10.00");
		strcpy(path, result_str2);
		result_str2 = strstr(path, "MBytes");
		result_str2 = strtok((result_str2+8), " ");
		float send_throughput = atof(result_str2);

		if(send_throughput >= 85.0)
		{
			printf("====================================================\n");
			printf("     [Success] Ethernet Throughput 10s Test !!\n");
			printf("====================================================\n");
			return 0;
		}
		else
		{
			printf("====================================================\n");
			printf("     [Failed] Ethernet Throughput 10s Test !!\n");
			printf("====================================================\n");
			return -1;
		}

	}
	else
	{
			printf("====================================================\n");
			printf("     [Failed] Ethernet Throughput 10s Test !!\n");
			printf("====================================================\n");
			return -1;
	}
	pclose(fp);
}

int main(int argc, char* argv[])
{
	int retval=0;
	int func_case;
	int i=0;
	char cmd[100];
	char ip[16];
	char target_ip[16];
	if(argc < 2)
	{
		printf("Usage : ./buzzer_test [CMD (ping/iperf)]\n");
		return 0;
	}


	while(i<argc)
	{
		if(!strcmp("ping", argv[i]))
		{
			func_case = 1;
			strcpy(target_ip, "192.168.0.1");
		}	
		else if(!strcmp("iperf", argv[i]))
		{
			func_case = 2;
			strcpy(target_ip, "192.168.0.2");
		}
		else if(!strcmp("-i", argv[i]))
		{
			if( (i+1) < argc)
			{
				
				//printf("argv : %s\n", argv[i+1]);
				strcpy(ip, argv[i+1]);
				sprintf(cmd, "ifconfig eth0 %s up\n", argv[i+1]);
				//printf("cmd : %s\n", cmd);
				retval = system(cmd);
			}
		}
		else if(!strcmp("-g", argv[i]))
		{
			printf("argv : %s\n", argv[i+1]);
			if( (i+1) < argc)
			{
				sprintf(cmd, "route add default gw %s\n", argv[i+1]);
				system(cmd);
			}
		}
		else if(!strcmp("-d", argv[i]))
		{
			system("killall -q dhcpcd\n");
			retval = system("dhcpcd\n");
		}
		else if(!strcmp("-t", argv[i]))
		{
			if( (i+1) < argc)
			{
				strcpy(target_ip, argv[i+1]);
			}
		}
		i++;
	}
	if( func_case == 0 )	
	{
		printf("Usage : ./buzzer_test [CMD (ping/iperf)]\n");
		return 0;
	}

	if(retval)
	{
		printf("[Error] Ethernet IP dosen't Set !!\n");
		printf("retval : %d\n", retval);
		return -1;
	}

	printf("====================================================\n");
	printf("[Ready] Ethernet Driver Setting to IP\n");
	printf("Start Test From %s(Board IP)\n", ip);
	printf("           To   %s(Target IP)\n", target_ip);
	printf("====================================================\n");
	switch (func_case)
	{
		case 1 : 
			{
				retval = ping_test(target_ip);
				break;
			}
		case 2 :
			{
				retval = iperf_test(target_ip);
				break;
			}
		default:
			break;
	}
	return retval;
}
