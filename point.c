#include "point.h"
#include "algorithm.h"

void gambarGaris(Point P1, Point P2, Matrix* M, char c) {
	int dx = absolute(P2.x-P1.x), sx = P1.x<P2.x ? 1 : -1;
	int dy = absolute(P2.y-P1.y), sy = P1.y<P2.y ? 1 : -1;
	if(P1.x != P2.x) {
		int err = (dx>dy ? dx : -dy)/2, e2;
		for(;;) {
			plotXY(M,P1.x,P1.y,c);
			if (P1.x==P2.x && P1.y==P2.y) break;
			e2 = err;
			if (e2 >-dx) { err -= dy; P1.x += sx; }
			if (e2 < dy) { err += dx; P1.y += sy; }
		}

	} else {
		int start = min(P1.y, P2.y);
		int finish = max(P1.y, P2.y);
		for(int i = start;i <= finish;i++) {
			plotXY(M,P1.x,i,c);
		}
	}
}

int isGarisPutus(Point P1, Point P2, Matrix* M, char c) {
    int ret = 0;
	int dx = absolute(P2.x-P1.x), sx = P1.x<P2.x ? 1 : -1;
	int dy = absolute(P2.y-P1.y), sy = P1.y<P2.y ? 1 : -1;
	if(P1.x != P2.x) {
		int err = (dx>dy ? dx : -dy)/2, e2;
		for(;ret == 0;) {
            if(P1.x >= 0 && P1.y >= 0 && P1.x < M->xsize && P1.y < M->ysize) {
                if (M->M[P1.y][P1.x] != c) {
                    ret=1;
                }
            }
			if (P1.x==P2.x && P1.y==P2.y) break;
			e2 = err;
			if (e2 >-dx) { err -= dy; P1.x += sx; }
			if (e2 < dy) { err += dx; P1.y += sy; }
		}

	} else {
		int start = min(P1.y, P2.y);
		int finish = max(P1.y, P2.y);
		for(int i = start;i <= finish && ret == 0;i++) {
			if(P1.x >= 0 && i >= 0 && P1.x < M->xsize && i < M->ysize) {
                if (M->M[i][P1.x] != c) {
                    ret = 1;
                }
            }
		}
	}
	return ret;
}



void setXY(Point* P, int x, int y) {
	P->x = x;
	P->y = y;
}

void plotXY(Matrix* M, int x, int y, char c) {
	if(x >= 0 && y >= 0 && x < M->xsize && y < M->ysize) {
		M->M[y][x] = c;
	}
}
