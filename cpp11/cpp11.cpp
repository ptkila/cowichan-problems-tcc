#include "cpp11.h"
#include <cstdlib>
#include <ctime>
#include <climits>

int main () {

	Cpp11 *cpp = new Cpp11();

	//Teste randmat
	//cpp->randmat(2, 2, 10);

	//Teste norm
	/*
	int numberOfPoints = 1000000;
	Point *points = new Point[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	cpp->norm(points, numberOfPoints);
	*/

	//Teste outer
	/*
	int numberOfPoints = 4;
	Point *points = new Point[numberOfPoints];
	srand (time(NULL));

	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
		points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
	
	}
	
	cpp->outer(points, numberOfPoints);
	*/
	// teste PRODUCT
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

	cpp->product(matrix, vector, matrixSize);	

	return 0;
}