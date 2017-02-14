#ifndef OBJECT_H
#define OBJECT_H
#include "point.h"
#include "lingkaran.h"
#include "matrix.h"

#define PI 3.14159265

typedef struct
{
	Point pointInit;
	Point P[30];
	Lingkaran L[2];
	int nlingkaran;
	int size;
} Object;

void setXYObject(Object* O, int x, int y);
void moveVertical(Object* O, int y);
void moveHorizontal(Object* O, int x);
void rotateClockwise(Object* O, int angle);
void rotateWheelClockwise(Object*, int);
void rotateCounterClockwise(Object* O, int angle);
void wheelBounce(Object*, int, int *isXtream);
int isWheelOut(Object*);
int isOut(Object *O, float rangex, float rangey);
void gambarObject(Object O, Matrix* M, char c);
int isObjectCollide(Object O, Matrix* M, char c);
Object makePeluru(int xinit, int yinit);
Object makePesawat(int xinit, int yinit);
Object makeLedakan(int xinit, int yinit);
Object makeLedakanPesawat1(int xinit, int yinit);
Object makeLedakanPesawat2(int xinit, int yinit);
Object makeLedakanPesawat3(int xinit, int yinit);
Object makeBaling(int xinit, int yinit);
Object makeMeriam(int xinitA, int yinitA);
Object makeWheel(int, int);
Object makeLine(int, int);
#endif
