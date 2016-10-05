#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "omp.h"

static double* matrix;
static double* target;
static double* solution;

void elimination(const int size) {

	//i = diagonal principal
	//j = linha abaixo de i
	//k = colunas de j

	int i, j, k;
	for (i = 0; i < size - 1; ++i) {
    	#pragma omp parallel shared(matrix, target, i) private(k)
    	{
    		#pragma omp for schedule(static)
	    	for (j = i + 1; j < size; ++j) {
	      		
	      		//Elemento que zera o valor abaixo da diag prin
	      		double mult = matrix[j*size + i]/ matrix[i*size + i];

	      		//Atualiza linha
	      		for (k = i; k < size; k++) {
					matrix[j*size + k] -= matrix[i*size + k] * mult;
	      		}
	      		// Atualiza vetor
	      		target[j] -= target[i] * mult;
	    	}
    	}
  	}
  	/*
  	// Testar eliminação
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%.0f\t", matrix[i*size + j]);
		}
		printf("\n");
	}
	printf("\n");
	*/		
}

void fill_solution (const int size) {

	// i = linha
	// j = coluna
	int i, j;
	for (i = size - 1; i >= 0; i--) {
    	solution[i] = target[i];
    	for (j = size - 1; j > i; j--) {
      		solution[i] -= matrix[i*size + j] * solution[j];
    	}
    	solution[i] /= matrix[i*size + i];
  	}
}

void gauss(const int size) {

    elimination(size);
    fill_solution(size);

}

void set_threads_number(const int n_threads) {

	omp_set_num_threads(n_threads);

}

void set_target_values(const int size) {

	int i;
	for (i = 0; i < size; ++i) {
		target[i] = (double)(rand() % 1000);
	}
	/*
	target[0] = 2;
	target[1] = 4;
	target[2] = 0;
	*/
}	

void set_matrix_values (const int size) {
	int  i, j;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (i == j) {
				matrix[i*size + j] = (double)(rand() % 1000);
			} else {
				matrix[i*size + j] = (double)(rand() % 100);
			}
		}
	}

	/*
	matrix[0] = 1;
	matrix[1] = 1;
	matrix[2] = 0;

	matrix[3] = 2;
	matrix[4] = -1;
	matrix[5] = 3;

	matrix[6] = -1;
	matrix[7] = 0;
	matrix[8] = 1;
	*/

	/*
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%d ", matrix[i*size + j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
}

int main (int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (double*) malloc (sizeof(double) * size * size);
		target = (double*) malloc (sizeof(double) * size);
		solution = (double*) calloc (sizeof(double), size);

		set_threads_number(n_threads);
		set_matrix_values(size);
		set_target_values(size);
		
		gauss(size);

		if (print == 1) {
			int i;
			for (i = 0; i < size; ++i) {
				printf("%f ", solution[i]);
			}
			printf("\n");
		}

		// Testar valores
		/*
		double* result = (double*) calloc (sizeof(double), size);
		int i, j;

		for (i = 0; i < size; ++i) {
     		for (j = 0; j < size; ++j) {
        		result[i] += matrix[i*size + j] * solution[j];
      		}
    	}

		for (i = 0; i < size; ++i) {
        	printf("%f = %f\n", result[i], target[i]);
    	}
		*/

		free(matrix);
		free(target);
		free(solution);

	} else {


	}

	return 0;
}