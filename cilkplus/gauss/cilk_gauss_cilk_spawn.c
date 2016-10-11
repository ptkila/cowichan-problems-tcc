#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static double* matrix;
static double* target;
static double* solution;

void elimination (const int begin, const int end, const int size, const int iter) {

	if (begin + 1 == end) {

		double mult = matrix[begin*size + iter]/ matrix[iter*size + iter];

      	int k;
      	for (k = iter; k < size; ++k) {
			matrix[begin*size + k] -= matrix[iter*size + k] * mult;
      	}
      	
      	target[begin] -= target[iter] * mult;

      	return;

	} else {

		int middle = begin + (end - begin)/ 2;
		cilk_spawn elimination(begin, middle, size, iter);
		cilk_spawn elimination(middle, end, size, iter);
		cilk_sync;
	
	}
}

void fill_solution (const int size) {

	int i, j;
	for (i = size - 1; i >= 0; --i) {
    	solution[i] = target[i];
    	for (j = size - 1; j > i; --j) {
      		solution[i] -= matrix[i*size + j] * solution[j];
    	}
    	solution[i] /= matrix[i*size + i];
  	}
}

void gauss(const int size) {

	int i;
	for (i = 0; i < size - 1; ++i) {
		cilk_spawn elimination(i + 1, size, size, i);
		cilk_sync;
	}
    
    fill_solution(size);
    
}

void set_threads_number(const int n_threads) {

	char threads[2];
	sprintf(threads,"%d", n_threads);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

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
		
		cilk_spawn gauss(size);
		cilk_sync;

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