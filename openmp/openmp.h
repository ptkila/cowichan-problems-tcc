#ifndef __OPENMP_H__
#define __OPENMP_H__

#include <stdio.h>
#include "omp.h"

struct point
{
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

void randmat (int nrows, int ncols, int s);

void shuffle (int *matrix);

void sor (double *matrix, double *target, double tolerance);

void thresh (int *matrix, double percent);

void vecdiff (double * left, double *right);

void winnow (int *matrix, _Bool *mask, int nelts);

#endif