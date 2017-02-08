#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

void initMatrix(Matrix *M, int x, int y) {
	M->xsize = x;
	M->ysize = y;
	M->M = (char**) malloc(y * sizeof(char*));
	for(int i = 0;i < y;i++) {
		M->M[i] = (char*) malloc(x * sizeof(char*));
	}
}

void resetMatrix(Matrix *M) {
	for(int i = 0;i < M->ysize;i++) {
		for(int j = 0;j < M->xsize;j++) {
			M->M[i][j] = 0;
		}
	}
}

void printMatrix(Matrix M) {
	for(int i = 0;i < M.ysize;i++) {
		for(int j = 0;j < M.xsize;j++) {
			printf("%c",M.M[i][j]);
		}
		printf("\n");
	}
}

void fillMatrix(Matrix *M,int x,int y, int color) {
    if( (x>=0) && (x < M->xsize) && (y>=0) && (y < M->ysize) && (M->M[y][x]==0) ) {
    	M->M[y][x]=color;
		fillMatrix(M,x+1,y,color);
		fillMatrix(M,x-1,y,color);
		fillMatrix(M,x,y+1,color);
		fillMatrix(M,x,y-1,color);
	}
}
