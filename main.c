// #include "garis.h"

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
struct timespec tim; 
 
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
 
void *get_keypress(void *x_void_ptr)
{
	while (end == 1) {
		c = getchar();		
	}
}

void *make_bullets(void *x_void_ptr) {
	while (end == 1) {
		if (c == '\n') {
			bullets[nBullets] = makePeluru(600,500);
			yBullet[nBullets] = 510;
			++nBullets;
			if (nBullets >= 19) {
				nBullets = 0;
			}
			c = 'p';
		}
	}
}

int main(){
   	int x = 0, y = 0;
	
	// Open the file for reading and writing
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



//===============================================================================

	pthread_t thread_keypress, thread_bullet;

	if(pthread_create(&thread_keypress, NULL, get_keypress, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	if(pthread_create(&thread_bullet, NULL, make_bullets, NULL)) {
		fprintf(stderr, "Error creating thread 2\n");
		return 1;
	}
//----------------------------------------------------------------------------------

	Matrix M;
	Point P1, P2;
	Lingkaran L;
	char c1, c2, c3, c4, c5, c6, c7;
	c1 = '1';
	c2 = '2';
	c3 = '3';
	c4 = '4';
	c5 = '5';
	c6 = '6';
	c7 = '7';
	initMatrix(&M, 1200, 700);
	resetMatrix(&M);

	Object pesawat = makePesawat(950,100);
	Object ledakan;
	Object ledakan1;
	Object ledakan2;
	Object ledakan3;
	Object meriam = makeMeriam(600,750);

	gambarObject(pesawat, &M, c1);
    gambarObject(meriam, &M, c3);
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

//---------------
	int collide = 0;
	int xPesawat = 1100;
	//the main display, game ends when bullet collides with plane
	do {
		moveHorizontal(&pesawat,-10);
		int j;
		for (j = 0; j < nBullets; ++j) {
			moveVertical(&bullets[j], -15);
			yBullet[j] -=15;
		}
		resetMatrix(&M);
		gambarObject(pesawat, &M, c1);
	   	gambarObject(meriam, &M, c3);
	   	gambarObject(ledakan, &M, c4);
		for (j = 0; j < nBullets; ++j) {
			gambarObject(bullets[j], &M, c2);
		}
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

		for (j = 0; j < nBullets; ++j) {
			fill(600,yBullet[j],WHITE);
		}

		xPesawat -= 10;
		fill (xPesawat, 100, WHITE); // pesawat
		
		//check if plane is out of screen
		if (isOut(&pesawat,-300,0)){
    		moveHorizontal(&pesawat,1500);
    		xPesawat = 1300;
    	}
		
		//check collide condition
		if (isObjectCollide(pesawat, &M, c1) == 1) {
			ledakan = makeLedakan(550,100);
			pesawat = makePesawat(1500,100);
			ledakan1 = makeLedakanPesawat1(550,100);
			ledakan2 = makeLedakanPesawat2(550,100);
			ledakan3 = makeLedakanPesawat3(550,110);
			collide = 1;

			Point l1;
			Point l2;
			Point l3;
			l1.x = 570;
			l1.y = 100;
			l2.x = 570;
			l2.y = 100;
			l3.x = 570;
			l3.y = 100;

			int dx = 0;
			while(isOut(&ledakan2,0,1000)){
				dx++;
				moveHorizontal(&ledakan1,-3);
				moveVertical(&ledakan1,((dx*dx)+2*dx)/1000);
				l1.x += -3;
				l1.y +=((dx*dx)+2*dx)/1000;
				rotateCounterClockwise(&ledakan1,15);
				// rotatePoint(&l1,550,100,-15);

				moveHorizontal(&ledakan2,2);
				moveVertical(&ledakan2,((dx*dx)+2*dx)/1500);
				l2.x += 2;
				l2.y +=((dx*dx)+2*dx)/1500;
				rotateClockwise(&ledakan2,5);
				// rotatePoint(&l2,550,100,5);

				moveHorizontal(&ledakan3,5);
				moveVertical(&ledakan3,((dx*dx)+2*dx)/1200);
				l3.x += 5;
				l3.y +=((dx*dx)+2*dx)/1200;
				rotateClockwise(&ledakan3,10);
				// rotatePoint(&l3,550,110,10);

				resetMatrix(&M);
				gambarObject(meriam, &M, c3);
				gambarObject(ledakan, &M, c4);
				gambarObject(ledakan1, &M, c5);
				gambarObject(ledakan2, &M, c5);
				gambarObject(ledakan3, &M, c5);
				// fill (l1.x,l3.y,WHITE);
				// fill (l2.x,l3.y,WHITE);
				// fill (l3.x,l3.y,WHITE);

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
				fill (550, 170, RED);	// ledakan
			}
		}
		fill (600, 700, RED);	// meriam bawah
		fill (600, 680, GREEN);	// meriam atas
		fill (600, 620, BLUE);	// meriam persegi panjang
		
		tim.tv_sec = 0;
		tim.tv_nsec = 100000000;
		nanosleep(&tim, NULL);
	} while (collide == 0);

    //closing connection
    end = 0;
    munmap(fbp, screensize);
    close(fbfd);

	if(pthread_join(thread_keypress, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}
	if(pthread_join(thread_bullet, NULL)) {
		fprintf(stderr, "Error joining thread 2\n");
		return 2;
	}
    return 0;
}
