grafikamake: main.c algorithm.c lingkaran.c matrix.c object.c point.c
	gcc main.c algorithm.c lingkaran.c matrix.c object.c point.c -o test -lpthread -std=c11 -lm