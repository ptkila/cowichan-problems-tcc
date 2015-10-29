#ifndef __CILK_PLUS_H__
#define __CILK_PLUS_H__

#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct point {
	double x;
	double y;
};

void gauss (double *matrix, double *target);

void hull (struct point *original);

void invperc (int *matrix, int nfill);

void life (_Bool *matrix, int numgen);

void mandel (int nrows, int ncols, int x0, int y0, int dx, int dy);

void norm (struct point *points, int number_of_points);

void outer (struct point *points, int number_of_points);

void product (double **matrix, double *vector, int size);

void shuffle (int *matrix);

void sor (double *matrix, double *target, double tolerance);

void thresh(int** matrix, int size, int percent, int** mask);

void vecdiff (double * left, double *right);

void winnow (int *matrix, _Bool *mask, int nelts);

#endif