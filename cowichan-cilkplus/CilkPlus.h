#ifndef __CILK_PLUS_H__
#define __CILK_PLUS_H__

#include <cilk/cilk.h>
#include <stdio.h>

void gauss (double *matrix, double *target);

void hull (struct point *original);
	
void invperc (int *matrix, int nfill);

void life (_Bool *matrix, int numgen);

void mandel (int nrows, int ncols, int x0, int y0, int dx, int dy);

void norm (struct point *points);
	
void outer (struct point *points);

void product (double *matrix, double *actual, double *candidate);

void shuffle (int *matrix);

void sor (double *matrix, double *target, double tolerance);

void thresh (int *matrix, double percent);

void vecdiff (double * left, double *right);

void winnow (int *matrix, _Bool *mask, int nelts);

#endif