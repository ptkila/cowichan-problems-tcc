#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static double* matrix;
static double* vector;
static double* result;
static int n_threads;

void product (const int size) {
	int i, j;
	CILK_C_REDUCER_OPADD(sum, double, 0.0);
	CILK_C_REGISTER_REDUCER(sum);
	cilk_for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			REDUCER_VIEW(sum) += matrix[i*size + j] * vector [j];
		}
		result[i] = REDUCER_VIEW(sum);
		REDUCER_VIEW(sum) = 0.0;
	}
	CILK_C_UNREGISTER_REDUCER(sum);
}

void set_values_matrix(const int size) {
	int i, j;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			matrix[i*size + j] = (float) rand();
		}
	}
}

void set_values_vector(const int size) {
	int i;
	for (i = 0; i < size; ++i) {
		vector[i] = (float) rand();
	}
}

void set_threads_number() {

	char threads[2];
	sprintf(threads,"%d", n_threads);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (double*) malloc (sizeof(double) * size * size);
		result = (double*) calloc (sizeof(double), size);
		vector = (double*) malloc (sizeof(double) * size);

		set_threads_number();
		set_values_matrix(size);
		set_values_vector(size);
		product(size);

		if (print == 1) {
			int i;
			for (i = 0; i < size; ++i) {
				printf("%g ", result[i]);
			}
			printf("\n");
		}

		free(matrix);
		free(vector);
		free(result);

	} else {

		printf("programa <tamanho> <num de num_threads> <printar>\n");

	}

	return 0;
}