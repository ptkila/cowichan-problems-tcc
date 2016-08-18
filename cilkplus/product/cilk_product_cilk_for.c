#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static double *matrix;
static double *vector;
static double *result;

void product (int nelts) {
	cilk_for (int i = 0; i < nelts; ++i) {
		double sum = 0;
		for (int j = 0; j < nelts; ++j) {
			sum += matrix [i*nelts + j] * vector [j];
		}
		result [i] = sum;
	}
}

void set_values_matrix(int nelts) {
	int i, j;
	for (i = 0; i < nelts; i++) {
		for (j = 0; j < nelts; j++) {
			matrix[i*nelts + j] = (float) rand();
		}
	}
}

void set_values_vector(int nelts) {
	int i;
	for (i = 0; i < nelts; i++) {
		vector[i] = (float)rand();
	}
}

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char *argv[]) {

	srand (time(NULL));
	int nelts = atoi(argv[1]);
	int num_threads = atoi(argv[2]);
	int print = atoi(argv[3]);
	int i;
	
	if (argc == 4) {

		matrix = (double*) malloc (sizeof(double) * nelts * nelts);
		vector = (double*) malloc (sizeof(double) * nelts);
		result = (double*) malloc (sizeof(double) * nelts);

		set_values_matrix(nelts);
		set_values_vector(nelts);
		set_threads_number(num_threads);
		product(nelts);

		if (print == 1) {
			for (i = 0; i < nelts; i++) {
				printf("%g ", result[i]);
			}
			printf("\n");
		}

	} else {

		printf("programa <tamanho> <num de num_threads> <printar>\n");

	}

	return 0;
}