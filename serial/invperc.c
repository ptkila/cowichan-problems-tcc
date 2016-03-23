#include "serial.h"

#define N_SIDES 4

void set_middle_point (int** mask, int size) {

	mask[size/2][size/2] = 1;

}

void percolate (int** mask, int** matrix, int size) {
	
	struct point_int found;
	int found_value, x, y, side, x_neib, y_neib;
	int x_steps[4] = {1, 0, -1, 0};
	int y_steps[4] = {0, 1, 0, -1};

	found.x = -1;
	found.y = -1;
	found_value = INT_MAX;

	for (x = 0; x < size ; ++x) {
		for (y = 0; y < size; ++y) {
			if (mask[x][y] == 1) {
				for (side = 0; side < N_SIDES; ++side) {
					x_neib = x + x_steps[side];
					y_neib = y + y_steps[side];
					if (x_neib < size && x_neib >= 0 && 
						y_neib < size && y_neib >= 0 &&
						mask[x_neib][y_neib] == 0 && matrix[x_neib][y_neib] < found_value){
						found.x = x_neib;
						found.y = y_neib;
						found_value = matrix[x_neib][y_neib];
					}
				}
			}
		}
	}

	if (found.x >= 0 && found.y >= 0) {
		mask[found.x][found.y] = 1;
	}
}

int** invperc (int **matrix, int size, int nfill) {

	int** mask;
	int i, j, percentage;

	mask = (int**)calloc(size, sizeof(int*));
	for (i = 0; i < size; ++i){
		mask[i] = (int*)calloc(size, sizeof(int));
	}
	
	set_middle_point(mask, size);
	percentage = (size * size * nfill) /100;

	for (i = 0; i < percentage; ++i) {

		percolate(mask, matrix, size);
	
	}
	/*
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j)
		{
			printf("%d ", mask[i][j]);
		}
		printf("\n");
	}
	*/
	return mask;
}