#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int* matrix;
static int* tmpMatrix;

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

void update_matrix(const int size) {
	int i, j;
	cilk_for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			matrix[i* size + j] = tmpMatrix[i*size + j];
		}
	}
}

void evaluate_matrix (const int size) {
	int i, j;
	int count = 0;
	cilk_for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			
			count = get_neighbor_count(size, i, j);

			if (matrix[i*size + j] == 1) {
				tmpMatrix[i*size + j] = (count == 2 || count == 3) ? 1 : 0;
			} else {
				tmpMatrix[i*size + j] = (count == 3) ? 1 : 0;
			}
		}
	}
}

void play(const int size) {

	evaluate_matrix(size);
	update_matrix(size);

}

void life (const int size, const int numgen) {

	int i = 0;
	for (i = 0; i < numgen; ++i) {
		play(size);
	}	
}

void set_threads_number(const int n_threads) {

	char threads[2];
	sprintf(threads,"%d", n_threads);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

}

void set_matrix_values (const int size) {
	int  i, j;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 2;
		}
	}
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (int*) malloc (sizeof(int) * size * size);
		tmpMatrix = (int*) malloc (sizeof(int) * size * size);
		int numgen = 5;

		set_threads_number(n_threads);
		set_matrix_values(size);
		life(size, numgen);

		if (print == 1) {
			int i, j;
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
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