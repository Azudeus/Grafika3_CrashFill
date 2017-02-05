// #include "garis.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

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
 
void *get_keypress(void *x_void_ptr)
{
	while (end == 1) {
		c = getchar();
	}
}

void *make_bullets(void *x_void_ptr) {
	while (end == 1) {
		if (c == 'p') {
			bullets[nBullets] = makePeluru(600,500);
			++nBullets;
			if (nBullets >= 19) {
				nBullets = 0;
			}
		}
	}
}

int main(){
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
   	int x = 0, y = 0;
    long int location = 0;
	
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
	char c1, c2, c3, c4;
	c1 = '1';
	c2 = '2';
	c3 = '3';
	c4 = '4';
	initMatrix(&M, 1200, 700);
	resetMatrix(&M);

	Object pesawat = makePesawat(950,100);
	Object ledakan = makeLedakan(1500,100);
	Object meriam = makeMeriam(600,750);

	gambarObject(pesawat, &M, c1);
    gambarObject(meriam, &M, c3);
    gambarObject(ledakan, &M, c4);

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
	
	//the main display, game ends when bullet collides with plane
	do {
		moveHorizontal(&pesawat,-2);
		int j;
		for (j = 0; j < nBullets; ++j) {
			moveVertical(&bullets[j], -2);
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
		
		//check if plane is out of screen
		if (isOut(&pesawat,-300,0)){
    		moveHorizontal(&pesawat,1500);
    	}
		
		//check collide condition
		if (isObjectCollide(pesawat, &M, c1) == 1) {
			ledakan = makeLedakan(550,100);
			pesawat = makePesawat(1500,100);
			collide = 1;
			
			//what to do next
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
		}
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
