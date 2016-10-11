#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "omp.h"

static double* matrix;
static double* target;
static double* solution;

void elimination(const int size) {

	int i, j, k;
	for (i = 0; i < size - 1; ++i) {
    	#pragma omp parallel shared(matrix, target, i) private(k)
    	{
    		#pragma omp for schedule(static)
	    	for (j = i + 1; j < size; ++j) {
	      		
	   	      	double mult = matrix[j*size + i]/ matrix[i*size + i];

	      		for (k = i; k < size; k++) {
					matrix[j*size + k] -= matrix[i*size + k] * mult;
	      		}
	      		target[j] -= target[i] * mult;
	    	}
    	}
  	}	
}

void fill_solution (const int size) {

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

		free(matrix);
		free(target);
		free(solution);

	} else {


	}

	return 0;
}