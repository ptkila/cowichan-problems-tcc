#include "serial.h"

#define VAL_A 1313
#define VAL_B 3131

void calculate_rand_number (int** matrix, int nrows, int ncols, int s) {

  int i, j, seed = 0;
  
  for (i = 0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      seed = s + i + j;
      seed = VAL_A * seed + VAL_B;
      matrix[i][j] = seed % 100;
    }
  }
}

int** randmat (int nrows, int ncols, int s) {

  int i, j, **matrix;
  
  matrix = (int**)malloc((nrows) * sizeof(int*));
  for (int i = 0; i < nrows; ++i) {
    matrix[i] = (int*)malloc((ncols) * sizeof(int));
  }

  calculate_rand_number(matrix, nrows, ncols, s);
 
 /*
  for (i = 0; i < nrows; i++) {
      for (j = 0; j < ncols; j++) {
        printf("%d ", matrix[i][j]);
      }
      printf("\n");
  }
 */
  
 return matrix;
}