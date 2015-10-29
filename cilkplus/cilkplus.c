#include "cilkplus.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main () {

	//randmat(2, 2, 10);

	/*
	int number_of_points = 8;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	norm(points, number_of_points);
	*/
	
	//Teste outer
	/*
	int number_of_points = 4;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	
	srand (time(NULL));
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
		points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
	
	}

	outer(points, number_of_points);
	*/

	//Teste product

	int matrix_size = 4;
	int i = 0, j = 0;
	double **matrix = (double **) malloc (sizeof(double *)*matrix_size);
	for (i = 0; i < matrix_size; i++) {
		matrix[i] = (double *)malloc(matrix_size * sizeof(double));
	}
	double *vector = (double *)malloc (sizeof(double) * matrix_size);

	for (i = 0 ; i < matrix_size; i++) {
		vector[i] = i;
		for (j = 0 ; j < matrix_size; j++) {
			matrix[i][j] = i + j;
		}
	}

	product(matrix, vector, matrix_size);

	return 0;

}