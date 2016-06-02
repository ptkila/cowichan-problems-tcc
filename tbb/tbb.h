#ifndef __tbb_h__
#define __tbb_h__
#include <iostream>
#include <cmath>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/blocked_range2d.h"
#include "tbb/parallel_reduce.h"

class TBB {

public:

	void norm (class Point *points, int numberOfPoints);

	void outer (class Point *points, int numberOfPoints);

	void product (double **matrix, double *vector, int size);

	void randmat (int nrows, int ncols, int s);

	void thresh(int** matrix, int size, int percent, int** mask);

};

class Point {

public:

	double x;
	double y;

	Point() {
		this->x = 0.0;
		this->y = 0.0;
	}

	Point (double x, double y) {
		this->x = x;
		this->y = x;
	}
};

#endif