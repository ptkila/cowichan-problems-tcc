#ifndef __CILK_PLUS_H__
#define __CILK_PLUS_H__

#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
	double x;
	double y;
};

struct outer_struct {
	double** matrix;
	double* vector;
};

int** randmat (int nrows, int ncols, int s, int num_threads, int should_print);

double** product (double **matrix, double *vector, int size, int num_threads, int should_print);

struct point* norm (struct point* points, int number_of_points, int num_threads, int should_print);

struct outer_struct outer (struct point *points, int number_of_points, int num_threads, int should_print);

int** thresh(int** matrix, int size, int percent, int** mask, int num_threads, int should_print);

#endif