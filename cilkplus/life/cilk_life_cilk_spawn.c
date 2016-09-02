#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int* matrix;
static int* tmpMatrix;
static int numgen;
static int n_threads; 

int get_neighbor_value(const int size, const int row, const int col) {

	if((row < 0 || row >= size) || (col < 0 || col >= size))
		return 0;
	return (matrix[row*size + col] == 1) ? 1 : 0;

}

int get_neighbor_count(const int size, const int row, const int col) {

	int count = 0;

	count += get_neighbor_value(size, row - 1, col - 1);
	count += get_neighbor_value(size, row - 1, col);
	count += get_neighbor_value(size, row - 1, col + 1);
	count += get_neighbor_value(size, row, col - 1);
	count += get_neighbor_value(size, row, col + 1);
	count += get_neighbor_value(size, row + 1, col - 1);
	count += get_neighbor_value(size, row + 1, col);
	count += get_neighbor_value(size, row + 1, col + 1);

	return count;

}

void update_matrix(const int begin, const int end, const int size) {
	
	if(begin + 1 == end) {
		
		int i;	
		for (i = 0; i < size; ++i) {
			matrix[begin*size + i] = tmpMatrix[begin*size + i];
		}
		return;
	
	} else {

		int middle = begin + (end - begin)/ 2;
		cilk_spawn update_matrix(begin, middle, size);
		cilk_spawn update_matrix(middle, end, size);
		cilk_sync;
	
	}
}

void evaluate_matrix (const int begin, const int end, const int size) {	
	if (begin + 1 == end) {
		int i, count;
		for (i = 0; i < size; ++i) {
			count = get_neighbor_count(size, begin, i);
			if (matrix[begin*size + i] == 1) {
				tmpMatrix[begin*size + i] = (count == 2 || count == 3) ? 1 : 0;
			} else {
				tmpMatrix[begin*size + i] = (count == 3) ? 1 : 0;
			}
		}
		return;
	
	} else {

		int middle = begin + (end - begin)/ 2;
		cilk_spawn evaluate_matrix(begin, middle, size);
		cilk_spawn evaluate_matrix(middle, end, size);
		cilk_sync;

	}
}

void play(const int size) {

	//int i, j;

	cilk_spawn evaluate_matrix(0, size, size);
	cilk_sync;

	cilk_spawn update_matrix(0, size, size);
	cilk_sync;
	
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

void life (const int size, const int numgen) {
	int i = 0;
	for (i = 0; i < numgen; i++) {
		play(size);
	}	
}

void set_threads_number() {

	char threads[2];
	sprintf(threads,"%d", n_threads);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

}

void set_matrix_values (const int size) {
	int  i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrix[i*size + j] = rand() % 2;
		}
	}
	
	for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++) {
				printf("%d ", matrix[i*size + j]);
			}
			printf("\n");
	}
	printf("\n");
	
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (int*) malloc (sizeof(int) * size * size);
		tmpMatrix = (int*) malloc (sizeof(int) * size * size);
		numgen = 4;

		set_threads_number();
		set_matrix_values(size);
		
		cilk_spawn life(size, numgen);
		cilk_sync;

		if (print == 1) {
			int i, j;
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					printf("%d ", matrix[i*size + j]);
				}
				printf("\n");
			}
		}

		free(matrix);
		free(tmpMatrix);

	} else {


	}

	return 0;
}