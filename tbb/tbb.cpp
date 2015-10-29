#include "tbb.h"
#include <cstdlib>
#include <ctime>
#include <climits>

int main () {

	TBB *tbb = new TBB();
	//teste randmat
	//tbb->randmat(2,2,10);
	
	//teste norm
	/*
	int numberOfPoints = 8;
	Point *points = new Point[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);
	
	}
	tbb->norm(points, numberOfPoints);
	*/

	//teste outer
	/*
	int numberOfPoints = 4;
	Point *points = new Point[numberOfPoints];
	srand (time(NULL));
	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
		points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
	
	}
	tbb->outer(points, numberOfPoints);
	*/

	//Teste product
	int matrixSize = 4;
	double **matrix = new double*[matrixSize];
	double *vector = new double[matrixSize];

	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new double[matrixSize];
	}

	for (int i = 0 ; i < matrixSize; i++) {
		vector[i] = i;
		for (int j = 0 ; j < matrixSize; j++) {
			matrix[i][j] = i + j;
		}
	}

	tbb->product(matrix, vector, matrixSize);
	
	return 0;
}