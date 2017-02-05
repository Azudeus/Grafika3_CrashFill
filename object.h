#ifndef OBJECT_H
#define OBJECT_H
#include "point.h"
#include "lingkaran.h"
#include "matrix.h"

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
int isOut(Object *O, int rangex, int rangey);
void gambarObject(Object O, Matrix* M, char c);
int isObjectCollide(Object O, Matrix* M, char c);
Object makePeluru(int xinit, int yinit);
Object makePesawat(int xinit, int yinit);
Object makeLedakan(int xinit, int yinit);
Object makeMeriam(int xinitA, int yinitA);
#endif
