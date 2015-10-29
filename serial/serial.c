#include "serial.h"

int main() {

	// teste randmat
	//randmat(2,2,10);

	// teste norm
	/*
	int number_of_points = 3;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	norm(points, number_of_points);
	*/

	// teste invperc
	/*
	int matrix_size = 3;
	int i = 0, j = 0;
	int **matrix = (int **) malloc (sizeof(int *)*matrix_size);
	for (i = 0; i < matrix_size; i++) {
		matrix[i] = (int *)malloc(matrix_size * sizeof(int));
	}

	for (i = 0 ; i < matrix_size; i++) {
		for (j = 0 ; j < matrix_size; j++) {
			matrix[i][j] = i;
		}
	}
	*/

	//Teste outer
	/*
	int number_of_points = 3;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)(i + 1);
		points[i].y = (double)(i + 2);

	}

	outer(points, number_of_points);
	*/

	//Teste product
	/*
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
	*/

	//teste thresh
	int matrix_size = 12;
	int i = 0, j = 0;
	int percent = 100;
	int **matrix = (int **) malloc (sizeof(int *)*matrix_size);
	for (i = 0; i < matrix_size; i++) {
		matrix[i] = (int *)malloc(matrix_size * sizeof(int));
	}

	int **mask = (int **)malloc (sizeof(int *) * matrix_size);
	for (i = 0; i < matrix_size; i++) {
		mask[i] = (int *)malloc(matrix_size * sizeof(int));
	}

	srand (time(NULL));

	for (i = 0 ; i < matrix_size; i++) {
		for (j = 0 ; j < matrix_size; j++) {
			matrix[i][j] = rand() % 255;
		}
	}

	thresh(matrix, matrix_size, percent, mask);

	return 0;
}