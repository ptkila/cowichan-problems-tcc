#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static double* matrix;
static double* target;
static double* solution;

/*
void elimination(const int size, const int iter) {

	//i = diagonal principal
	//j = linha abaixo de i
	//k = colunas de j

	int j, k;
    	cilk_for (j = iter + 1; j < size; ++j) {   		
      		//Elemento que zera o valor abaixo da diag prin
      		double mult = matrix[j*size + iter]/ matrix[iter*size + iter];

      		//Atualiza linha
      		for (k = iter; k < size; ++k) {
				matrix[j*size + k] -= matrix[iter*size + k] * mult;
      		}
      		// Atualiza vetor
      		target[j] -= target[iter] * mult;
      		//pthread_mutex_unlock(&m);
  	}

  	// Testar eliminação
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%.1f\t", matrix[i*size + j]);
		}
		printf("\n");
	}

*/

void elimination (const int begin, const int end, const int size, const int iter) {

	if (begin + 1 == end) {

		double mult = matrix[begin*size + iter]/ matrix[iter*size + iter];

      	//Atualiza linha
      	int k;
      	for (k = iter; k < size; ++k) {
			matrix[begin*size + k] -= matrix[iter*size + k] * mult;
      	}
      	
      	// Atualiza vetor
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

	// i = linha
	// j = coluna

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
		
		cilk_spawn gauss(size);
		cilk_sync;

		if (print == 1) {
			int i;
			for (i = 0; i < size; ++i) {
				printf("%f ", solution[i]);
			}
			printf("\n");
		}

		/*
		// Testar valores
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