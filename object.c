#include "object.h"
#include "lingkaran.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void setXYObject(Object* O, int x, int y) {
	O->pointInit.x = x;
	O->pointInit.y = y;
}

void moveVertical(Object* O, int y) {
	O->pointInit.y = O->pointInit.y + y;
}

void moveHorizontal(Object* O, int x) {
	O->pointInit.x = O->pointInit.x + x;
}

int isOut(Object* O,int rangex, int rangey) {
	if ((O->pointInit.x<=rangex)||(O->pointInit.y<=rangey)){
		return 1;
	}else{
		return 0;
	}
}


void gambarObject(Object O, Matrix* M, char c) {
	Point start, finish;
	for(int i = 0;i < O.size-1 ;i++) {
		setXY(&start, O.P[i].x + O.pointInit.x, O.P[i].y + O.pointInit.y);
		setXY(&finish, O.P[i+1].x + O.pointInit.x, O.P[i+1].y + O.pointInit.y);
		gambarGaris(start, finish, M, c);
	}

	setXY(&start, O.P[O.size - 1].x + O.pointInit.x, O.P[O.size - 1].y + O.pointInit.y);
	setXY(&finish, O.P[0].x + O.pointInit.x, O.P[0].y + O.pointInit.y);
	gambarGaris(start, finish, M, c);

	for(int i = 0;i < O.nlingkaran ;i++) {
        Lingkaran L;
        L.x = O.L[i].x + O.pointInit.x;
        L.y = O.L[i].y + O.pointInit.y;
        L.r = O.L[i].r;
		gambarLingkaran(&L, M, c);
	}
}

int isObjectCollide(Object O, Matrix* M, char c) {
    int ret = 0;
	Point start, finish;
	for(int i = 0;i < O.size-1 && ret == 0;i++) {
		setXY(&start, O.P[i].x + O.pointInit.x, O.P[i].y + O.pointInit.y);
		setXY(&finish, O.P[i+1].x + O.pointInit.x, O.P[i+1].y + O.pointInit.y);
		ret = isGarisPutus(start, finish, M, c);
	}
    if (ret == 0) {
        setXY(&start, O.P[O.size - 1].x + O.pointInit.x, O.P[O.size - 1].y + O.pointInit.y);
        setXY(&finish, O.P[0].x + O.pointInit.x, O.P[0].y + O.pointInit.y);
        ret = isGarisPutus(start, finish, M, c);
    }
    if (ret == 0) {
        Lingkaran L;
        for(int i = 0;i < O.nlingkaran && ret == 0;i++) {
            L.x = O.L[i].x + O.pointInit.x;
            L.y = O.L[i].y + O.pointInit.y;
            L.r = O.L[i].r;
            ret = isLingkaranPutus(&L, M, c);
        }
    }
    return ret;
}

Object makePeluru(int xinit, int yinit) {
	int x[6] = {2, 6, 6, -6, -6, -2};
	int y[6] = {0, 8, 24, 24, 8, 0};
	Object O;
	for(int i = 0;i < 6;i++) {
		O.P[i].x = x[i];
		O.P[i].y = y[i];
	}

	O.pointInit.x = xinit;
	O.pointInit.y = yinit;
	O.size = 6;
	O.nlingkaran = 0;
	return O;
}

Object makePesawat(int xinit, int yinit) {
	int x[22] = {0, 16, 81, 125, 134, 110, 112, 154, 167, 176, 169, 169, 176, 167, 154, 112, 110, 134, 125, 81, 16, 0};
	int y[22] = {3, 11, 11, 57, 57, 21, 11, 9, 28, 28, 11, -11, -28, -28, -9, -11, -21, -57, -57, -11, -11, -3};
	Object O;
	for(int i = 0;i < 22;i++) {
		O.P[i].x = x[i];
		O.P[i].y = y[i];
	}

	O.pointInit.x = xinit;
	O.pointInit.y = yinit;
	O.size = 22;
	O.nlingkaran = 0;
	return O;
}

Object makeLedakan(int xinit, int yinit) {
	int x[20] = {0, 20, 46, 40, 84, 54, 86, 51, 72, 29, 28, 4, -34, -19, -60, -22, -67, -21, -47, -4};
	int y[20] = {0, 54, -2, 61, 39, 86, 96, 107, 145, 116, 148, 112, 117, 103, 107, 79, 64, 56, 8, 49};
	Object O;
	for(int i = 0;i < 20;i++) {
		O.P[i].x = x[i];
		O.P[i].y = y[i];
	}

	O.pointInit.x = xinit;
	O.pointInit.y = yinit;
	O.size = 20;
	O.nlingkaran = 0;
	return O;
}

Object makeMeriam(int xinitA, int yinitA) {
	int x[4] = {13, 13, -13, -13};
	int y[4] = {-117, -207, -207, -117};
	Object O;
	for(int i = 0;i < 4;i++) {
		O.P[i].x = x[i];
		O.P[i].y = y[i];
	}

	int xL[2] = {0, 0};
	int yL[2] = {0, 0};
	int rL[2] = {69, 117};
	for(int i = 0;i < 2;i++) {
		O.L[i].x = xL[i];
		O.L[i].y = yL[i];
		O.L[i].r = rL[i];
	}

	O.pointInit.x = xinitA;
	O.pointInit.y = yinitA;
	O.size = 4;
	O.nlingkaran = 2;


	return O;
}
