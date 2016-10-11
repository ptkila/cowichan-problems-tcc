#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "omp.h"

struct found_point {

	int row, col, value;

};

static int* matrix;
static int* mask;
static struct found_point* points;

static const int N_SIDES = 4;
static const int X_STEPS[4] = {1, 0, -1, 0};
static const int Y_STEPS[4] = {0, 1, 0, -1};

void set_mask_middle_point (const int size) {
	if (size % 2 == 0) {
		mask[(size*size)/2 + size/2] = 1;
	} else {
		mask[(size*size)/2] = 1;
	}
}

void reset_initial_values(const int n_threads) {
	int i;
	for (i = 0; i < n_threads; ++i) {
		points[i].row = -1;
		points[i].col = -1;
		points[i].value = INT_MAX;
	}
}

int set_new_point(const int size, const struct found_point& point) {
	if (point.row >= 0 && point.col >= 0) {	
		mask[point.row*size + point.col] = 1;
		return 0;
	} else {
		return 1;
	}
}

int evaluate_neighbors (const int row, const int col, const int size) {
	return !((row < 0 || row >= size) || (col < 0 || col >= size)) ? 1 : 0; 
}

struct found_point percolate (const int size) {

	int n_threads, i, j, sides;

	#pragma omp parallel shared(mask, matrix, points, n_threads, size) \
		private(j, sides)
	{	
		#pragma omp single
		{
			n_threads = omp_get_num_threads();
			reset_initial_values(n_threads);
		}
		int thread_num = omp_get_thread_num();
		#pragma omp for schedule(static)
		for (i = 1; i < size - 1 ; ++i) {
			for (j = 1; j < size - 1; ++j) {
				if (mask[i*size + j] == 1) {
					for (sides = 0; sides < N_SIDES; ++sides) {
						int row = i + X_STEPS[sides];
						int col = j + Y_STEPS[sides];
						int pos = row*size + col;
						if (mask[pos] == 0 && matrix[pos] < 
								points[thread_num].value) {
							points[thread_num].row = row;
							points[thread_num].col = col;
							points[thread_num].value = matrix[pos];
						}
					}
				}
			}
		}
	}

	struct found_point point = points[0];
	for (i = 1; i < n_threads; ++i) {
		if (point.value > points[i].value) {
			point = points[i];
		}
	}

	return point;
}

void invperc (const int size, const int nfill) {
	
	int i;
	for (i = 0; i < nfill; ++i){
		struct found_point point = percolate(size);
		if(set_new_point(size, point)) { break; }
	}
}

void set_matrix_values (const int size) {
	int  i, j;
	for (i = 0; i < size; ++i) {a
		for (j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 1000;
		}
	}
}

void set_threads_number(const int n_threads) {

	omp_set_num_threads(n_threads);

}

int main (int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (int*) malloc (sizeof(int) * size * size);
		mask = (int*) calloc (size * size, sizeof(int));
		points = (struct found_point*) malloc (sizeof(struct found_point)
			* n_threads);
		int nfill = 100000;

		set_threads_number(n_threads);
		set_matrix_values(size);
		set_mask_middle_point(size);
		invperc(size, nfill);

		if (print == 1) {
			int i, j;
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
					printf("%d ", mask[i*size + j]);
				}
				printf("\n");
			}
		}

		free(matrix);
		free(mask);
		free(points);

	} else {


	}

	return 0;
}