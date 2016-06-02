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