#ifndef __cpp11_h__
#define __cpp11_h__

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <functional>

class Cpp11 {

public:

	void gauss (double *matrix, double *target);

	void hull (class Point *original);

	void invperc (int *matrix, int nfill);

	void life (bool *matrix, int numgen);

	void mandel (int nrows, int ncols, int x0, int y0, int dx, int dy);

	void norm (class Point *points, int numberOfPoints);

	void outer (class Point *points, int numberOfPoints);

	void product (double **matrix, double *vector, int size);

	void randmat (int nrows, int ncols, int s);

	void shuffle (int *matrix);

	void sor (double *matrix, double *target, double tolerance);

	void thresh(int** matrix, int size, int percent, int** mask);

	void vecdiff (double *left, double *right);

	void winnow (int *matrix, bool *mask, int nelts);

};

class Point {

public:

	double x;
	double y;

	Point () {
		this->x = 0.0;
		this->y = 0.0;
	}

	Point (double x, double y) {
		this->x = x;
		this->y = x;
	}
};

#endif