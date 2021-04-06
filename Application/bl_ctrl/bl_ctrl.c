#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BL_PATH	"/sys/class/backlight/backlight"
#define SAVE_BL "/etc/bl_value"

int bl_set(int value){
        FILE *fp;
        char buf[256];
	int write_value;

        snprintf(buf, sizeof(buf), "%s/brightness", BL_PATH);
        fp = fopen(buf, "w");
        if(!fp){
                printf("Fail to set backlight\n");
                return -1;
        }
        fprintf(fp,"%d\n", value);
        fclose(fp);

        fp = fopen(SAVE_BL, "w+");
        if(!fp){
                printf("Fail to set backlight\n");
                return -1;
        }
        fprintf(fp,"%d\n", value);
        fclose(fp);
        return 1;
}

int bl_get(void){
        FILE *fp;
        char buf[256];
        int ret;

        snprintf(buf, sizeof(buf), "%s/actual_brightness", BL_PATH);
        fp = fopen(buf, "r");
        if(!fp){
                printf("Fail to get backlight\n");
                return -1;
        }
        fscanf(fp, "%d", &ret);
        fclose(fp);
        return ret;
}

int main(int argc, char * argv[])
{
	int bl;

	bl = bl_get();
	printf("backlight : %d\n", bl);

	if(argc > 1){
		bl = atoi(argv[1]);
		if(bl < 0 | bl > 100 ){
			printf("bl value is 0 < value or 100 > value\n");
			return 0;
		}
		bl_set(bl);
		printf("Set backlight : %d\n", bl);
	}
	return 0;
}
