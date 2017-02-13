
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>

#include "lingkaran.h"
#include "matrix.h"
#include "point.h"
#include "object.h"
#include <stdio.h>
#include <pthread.h>

char c;
int end = 1;
int nBullets = 0;
Object bullets[20];
int yBullet[20];
 
char *fbp = 0;
int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
long location;
 
#define RED 1
#define BLACK 2
#define WHITE 3
#define GREEN 4
#define BLUE 5

void fillColor(int color) {
	if(color == RED) {
		*(fbp + location) = 0; 
		*(fbp + location + 1) = 0;   
		*(fbp + location + 2) = 255;   
		*(fbp + location + 3) = 0;  
	}
	else if(color == BLACK) {
		*(fbp + location) = 0; 
		*(fbp + location + 1) = 0; 
		*(fbp + location + 2) = 0; 
		*(fbp + location + 3) = 0;   					
	}
	else if(color == GREEN) {
		*(fbp + location) = 0;    
		*(fbp + location + 1) = 255; 
		*(fbp + location + 2) = 0; 
		*(fbp + location + 3) = 0; 
	} 
	else if(color == WHITE) {
		*(fbp + location) = 255; 
		*(fbp + location + 1) = 255;  
		*(fbp + location + 2) = 255;
		*(fbp + location + 3) = 0; 
	}
	else if(color == BLUE) {
		*(fbp + location) = 255; 
		*(fbp + location + 1) = 0;  
		*(fbp + location + 2) = 0;
		*(fbp + location + 3) = 50; 
	}
}
 
void fill(int x, int y, int color) 
{
	int tempX, tempY;

	if (x>10 && y>10){
		
		tempX = x+1; tempY = y;
		location = (tempX+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	               (tempY+vinfo.yoffset) * finfo.line_length;
	    if (*(fbp+location)==0 && *(fbp+location+1)==0 && *(fbp+location+2)==0) {
			fillColor(color);
			fill(tempX, tempY, color);
		}
			
		tempX = x-1; tempY = y;
		location = (tempX+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	               (tempY+vinfo.yoffset) * finfo.line_length;
	    if (*(fbp+location)==0 && *(fbp+location+1)==0 && *(fbp+location+2)==0) {
			fillColor(color);
			fill(tempX, tempY, color);
		}
		
		tempX = x; tempY = y+1;
		location = (tempX+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	               (tempY+vinfo.yoffset) * finfo.line_length;
	    if (*(fbp+location)==0 && *(fbp+location+1)==0 && *(fbp+location+2)==0) {
			fillColor(color);
			fill(tempX, tempY, color);
		}
		
		tempX = x; tempY = y-1;
		location = (tempX+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	               (tempY+vinfo.yoffset) * finfo.line_length;
	    if (*(fbp+location)==0 && *(fbp+location+1)==0 && *(fbp+location+2)==0) {
			fillColor(color);
			fill(tempX, tempY, color);
		}
	}
} 

int main() {

	fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                        fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");


	Matrix M;
	int x, y;
	initMatrix(&M, 1200, 700);
	resetMatrix(&M);
	char c1, c2, c3, c4, c5, c6, c7;
	c1 = '1';
	c2 = '2';
	c3 = '3';
	c4 = '4';
	c5 = 240;
	c6 = 240;
	c7 = 240;
	Object wheel = makeWheel(100,100);
	gambarObject(wheel, &M, c5);
	
//----------------------------------------------------------------------------------

	x = 700; y = 1200;       // Where we are going to put the pixel

//---------------
	for (y = 0; y < 700; y++) {
        for (x = 0; x < 1200; x++) {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = M.M[y][x];        // Some blue
                *(fbp + location + 1) = M.M[y][x]; //15+(x-100)/2;     // A little green
                *(fbp + location + 2) = M.M[y][x]; //200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } else  { //assume 16bpp
                int b = 10;
                int g = (x-100)/6;     // A little green
                int r = 31-(y-100)/16;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
        }
    }
    int xWheel = 0;
	fill(7,7, WHITE);
	munmap(fbp, screensize);
    close(fbfd);
	return 0;
}