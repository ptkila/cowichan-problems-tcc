#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct found_point {
	int row, col, value;
};

static int* matrix;
static int* mask;
static int nfill;
static struct found_point found;

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

void reset_found_point() {
	found.row = -1;
	found.col = -1;
	found.value = INT_MAX;
}

int set_new_point(const int size) {
	if (found.row >= 0 && found.col >= 0) {	
		
		mask[found.row*size + found.col] = 1;
		return 0;
	
	} else {

		return 1;
	
	}
}

int percolate (const int size) {

	int i, j, sides, row, col;

	for (i = 1; i < size - 1; ++i) {
		for (j = 1; j < size - 1; ++j) {
			if (mask[i*size + j] == 1) {
				for (sides = 0; sides < N_SIDES; ++sides) {
					row = i + X_STEPS[sides];
					col = j + Y_STEPS[sides];
					int pos = row*size + col;
					if (mask[pos] == 0 && matrix[pos] < found.value) {
						found.row = row;
						found.col = col;
						found.value = matrix[pos]; 
					}
				}
			}
		}
	}
}

void invperc (const int size, const int nfill) {
	
	int i;
	//int j, k;
	for (i = 0; i < nfill; ++i){
		reset_found_point();
		percolate(size);
		if(set_new_point(size))
			break;

		/*
		for (k = 0; k < size; k++) {
			for (j = 0; j < size; j++) {
				printf("%d ", mask[k*size + j]);
			}
			printf("\n");
		}
		printf("\n");
		*/
	}
}

void set_matrix_values (const int size) {
	int  i, j;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 1000;
		}
	}
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

	if (argc == 3) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int print = atoi(argv[2]);

		matrix = (int*) malloc (sizeof(int) * size * size);
		mask = (int*) calloc (size * size, sizeof(int));
		nfill = rand() % (size * size);

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

		printf("programa <tamanho> <printar>\n");

	}

	return 0;
}