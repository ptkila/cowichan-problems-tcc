#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct found_point {

	int row, col, value;

};

static int* matrix;
static int* mask;

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

void set_initial_values(struct found_point& point) {
	point.row = -1;
	point.col = -1;
	point.value = INT_MAX;
}


struct found_point percolate (const int begin, const int end, const int size) {
	
	if (begin + 1 == end) {
		struct found_point point;
		set_initial_values(point);
		int i, sides;
		cilk_for (i = 1; i < size - 1 ; ++i) {
			if (mask[begin*size + i] == 1) {
				for (sides = 0; sides < N_SIDES; ++sides) {
					int row = begin + X_STEPS[sides];
					int col = i + Y_STEPS[sides];
					int pos = row*size + col;
					if (mask[pos] == 0 && matrix[pos] < point.value) {
						point.row = row;
						point.col = col;
						point.value = matrix[pos]; 
					}
				}
			}
		}
		return point;

	} else {

		int middle = begin + (end - begin)/ 2;
		struct found_point point_l = cilk_spawn percolate(begin, middle, size);
		struct found_point point_r = cilk_spawn percolate(middle, end, size);
		cilk_sync;
	
		return point_l.value < point_r.value ? point_l: point_r; 
	
	}
}

/*
struct found_point percolate (const int size) {
	int i, j, sides;
	struct found_point point;
	set_initial_values(point);
	cilk_for(i = 1; i < size - 1; ++i) { 
		for (j = 1; j < size - 1; ++j) {
			if (mask[i*size + j] == 1) {
				for (sides = 0; sides < N_SIDES; ++sides) {
					int row = i + X_STEPS[sides];
					int col = j + Y_STEPS[sides];
					int pos = row*size + col;
					if (mask[pos] == 0 && matrix[pos] < point.value) {
						point.row = row;
						point.col = col;
						point.value = matrix[pos]; 
					}
				}
			}
		}
	}
	return point;
}
*/

int set_new_point(const int size, const struct found_point point) {
	if (point.row >= 0 && point.col >= 0) {	
		mask[point.row*size + point.col] = 1;
		return 0;
	} else {
		return 1;
	}
}

void invperc (const int size, const int nfill) {
	
	int i;
	for (i = 0; i < nfill; ++i){
		/*
		int j, k;
		for (k = 0; k < size; ++k) {
			for (j = 0; j < size; ++j) {
				printf("%d ", mask[k*size + j]);
			}
			printf("\n");
		}
		printf("\n");
		*/
		struct found_point point = cilk_spawn percolate(1, size - 1, size);
		cilk_sync;

		//struct found_point point = percolate(size);
		if (set_new_point(size, point)) { break; }
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
			matrix[i*size + j] = rand() % 100;
		}
	}
	/*
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
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

		matrix = (int*) malloc (sizeof(int) * size * size);
		mask = (int*) calloc(size * size, sizeof(int));
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

	} else {


	}

	return 0;
}