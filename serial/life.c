#include "serial.h"

int get_neighbor_value(int **matrix, const int size, const int row,
                       const int col) {

	if ((row < 0 || row >= size) || (col < 0 || col >= size))
		return 0;
	return (matrix[row][col] == 1) ? 1 : 0;

}

int get_neighbor_count(int **matrix, const int size, const int row,
                       const int col) {

	int count = 0;

	count += get_neighbor_value(matrix, size, row - 1, col - 1);
	count += get_neighbor_value(matrix, size, row - 1, col);
	count += get_neighbor_value(matrix, size, row - 1, col + 1);
	count += get_neighbor_value(matrix, size, row, col - 1);
	count += get_neighbor_value(matrix, size, row, col + 1);
	count += get_neighbor_value(matrix, size, row + 1, col - 1);
	count += get_neighbor_value(matrix, size, row + 1, col);
	count += get_neighbor_value(matrix, size, row + 1, col + 1);

	return count;

}

void update_matrix(int** old_matrix, int** new_matrix, const int size) {
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			old_matrix[i][j] = new_matrix[i][j];
		}
	}

}

void play(int** matrix, const int size) {

	int i, j;
	int count;
	int** tmpMatrix;

	count = 0;
	tmpMatrix = (int**)malloc(size * sizeof(int*));
	for (i = 0; i < size; ++i) {
		tmpMatrix[i] = (int*)malloc(size * sizeof(int));
	}

	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {

			count = get_neighbor_count(matrix, size, i, j);

			/*
			 C   N                 new C
  			 1   0,1             ->  0  # Lonely
   			 1   4,5,6,7,8       ->  0  # Overcrowded
  			 1   2,3             ->  1  # Lives
   			 0   3               ->  1  # It takes three to give birth!
   			 0   0,1,2,4,5,6,7,8 ->  0  # Barren
			*/

			if (matrix[i][j] == 1)
				tmpMatrix[i][j] = (count == 2 || count == 3) ? 1 : 0;
			else
				tmpMatrix[i][j] = (count == 3) ? 1 : 0;

		}
	}
	update_matrix(matrix, tmpMatrix, size);
}

int** life (int** matrix, const int size, const int numgen) {

	int i = 0;

	for (i = 0; i < numgen; ++i) {

		play(matrix, size);

	}
	
	return matrix;
}