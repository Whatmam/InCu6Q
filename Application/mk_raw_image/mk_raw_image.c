#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define RGB(r,g,b) ((((r)<<16) | ((g)<<8) | ((b) <<0 )))
#define FILE_NAME "raw_image"

typedef enum {false, true} bool;

int main(int argc, char **argv){
	int i;
	int j;
	FILE *fp;
	struct fb_var_screeninfo varinfo;
	struct fb_fix_screeninfo fixinfo;
	int fd;
	int fb_num, r, g, b;
	char sys_cmd[50];
	bool btest_cmd = false;
	unsigned int color;
	unsigned int colors[] = {
		RGB(0,0,0),
		RGB(255,255,255),
		RGB(255,0,0),
		RGB(0,255,0),
		RGB(0,0,255),
		RGB(255,0,255),
		RGB(0,255,255),
		RGB(255,255,0),
	};

	int idx, x, y, sz;
	unsigned int *image;


	char * fbp;
	long int location = 0;

	if(argc ==3 ){
		if(!strcmp("test", argv[1]))
		{
			btest_cmd= true;
			fb_num = atoi(argv[2]);
		}
		else
			goto cmd_err;
	}
	else if(argc == 5)
	{
		fb_num = atoi(argv[1]);
		r = atoi(argv[2]);
		g = atoi(argv[3]);
		b = atoi(argv[4]);
	}
	else
		goto cmd_err;
	

	sprintf(sys_cmd, "/dev/fb%d", fb_num);
	//fd = open(sys_cmd, O_RDONLY);
	fd = open(sys_cmd, O_RDWR);
	memset(sys_cmd, '\0', sizeof(sys_cmd));
	memset((void*)&varinfo, 0x00, sizeof(varinfo));
	memset((void*)&fixinfo, 0x00, sizeof(fixinfo));

	if(ioctl(fd, FBIOGET_VSCREENINFO, &varinfo) == -1){
		close(fd);
		fprintf(stderr, "Error occured while getting variable screen information.\n");
		return -1;
	}
	if(ioctl(fd, FBIOGET_FSCREENINFO, &fixinfo) == -1){
		close(fd);
		fprintf(stderr, "Error occurred while getting fixed screen information. \n");
		return -1;
	}

	printf("width : %d, height : %d - %dbpp\n", varinfo.xres, varinfo.yres, varinfo.bits_per_pixel);

#if 1
	sz = varinfo.yres * varinfo.xres * sizeof(unsigned int);
	image = (unsigned int *)malloc(sz);

	for( idx=0; idx<8; idx++ )
	{
		int sx, sy, height, width;
			height = varinfo.yres / 4;
			width = varinfo.xres / 2;
		
		if(btest_cmd)
		{
			color = colors[idx];
		}
		else
		{
			color = RGB(r,g,b);
		}
		sx = idx / 4 * width;
		sy = idx % 4 * height;

		printf("Drawing area %d, (%d, %d) %d x %d\r\n", idx+1, sx, sy, width, height);

		for( y = sy; y < sy+height; y++ )
		{
			for( x = sx; x< sx+width; x++ )
			{
				image[x + y * varinfo.xres] = color;
			}
		}
	}	
	//for(i=0; i<varinfo.yres; i++){
	//		for(j=0; j<varinfo.xres; j++){
	//				//image[i * varinfo.xres + j] = RGB(r,g,b);
	//				*p++ = RGB(r,g,b);
	//		}
	//}

	fp = fopen(FILE_NAME, "w");	
	fwrite(image, sz, 1, fp);
	fclose(fp);
	free(image);
	
	sprintf(sys_cmd, "cat %s > /dev/fb%d", FILE_NAME, fb_num);
	system(sys_cmd);

#else
	long int screensize = 0;
	// Figure out the size of the screen in bytes
	screensize = varinfo.xres * varinfo.yres * varinfo.bits_per_pixel / 8;		

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((int)fbp == -1) {
		printf("Error: failed to map framebuffer device to memory\n");
		return -1;
	}

	printf("The framebuffer device was mapped to memory successfully.\n");


	for( idx=0; idx<8; idx++ )
	{
		int sx, sy, height, width;
		if(varinfo.yres > varinfo.xres)
		{
			height = varinfo.yres / 4;
			width = varinfo.xres / 2;
		}
		else
		{
			height = varinfo.yres / 4;
			width = varinfo.xres / 2;
		}
		
		if(btest_cmd)
		{
			color = colors[idx];
		}
		else
		{
			color = RGB(r,g,b);
		}
		sx = idx / 4 * width;
		sy = idx % 4 * height;

		printf("Drawing area %d, (%d, %d) %d x %d\r\n", idx+1, sx, sy, width, height);

		for( y = sy; y < sy+height; y++ )
		{
			for( x = sx; x< sx+width; x++ )
			{
				location = (x+varinfo.xoffset) * (varinfo.bits_per_pixel/8) + 
					   (y+varinfo.yoffset) * fixinfo.line_length;
			//	*(fbp + x + y * varinfo.xres) = color;
			//	*(fbp + location) = color;
				*(fbp + location) = b;
				*(fbp + location + 1) = g;
				*(fbp + location + 2) = r;
				*(fbp + location + 3) = 0;
				
			}
		}
	}	

	
	munmap(fbp, screensize);
#endif
	close(fd);

	return 1;

cmd_err:
	printf("Usage : ./mk_raw_image [FB_NUM] [R] [G] [B]\n");
	printf("\t./mk_raw_image test [FB_NUM]\n");
	printf("\tR,G,B : 0 ~ 255\n");
	return 0;
}
