#include "cilkplus.h"

int reduce_max (int** matrix, int size) {

  int i, j;
  int max = 0;
  CILK_C_REDUCER_MAX(r, int, 0);
  CILK_C_REGISTER_REDUCER(r);

  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      CILK_C_REDUCER_MAX_CALC(r, matrix[i][j]);
    }
  }
  max = r.value;
  CILK_C_UNREGISTER_REDUCER(r);

  return max;

}

void fill_histogram(int** matrix, int size, int* histogram) {
  int i, j;
  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      histogram[matrix[i][j]]++;
    }
  }
}

void fill_mask(int** matrix, int size, int threshold, int** mask) {
  int i, j;
  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i][j] = (int)(matrix[i][j] >= threshold);
    }
  }
}

void thresh(int** matrix, int size, int percent, int** mask) {
  int i, j;
  int n_max = 0;

  n_max = reduce_max(matrix, size);

  int* histogram = (int*)calloc((n_max + 1), sizeof(int));

  fill_histogram(matrix, size, histogram);
  
  int count = (size * size * percent) / 100;
  int prefixSum = 0;
  int threshold = n_max;

  for (i = n_max; i >= 0 && prefixSum <= count; i--) {
    prefixSum += histogram[i];
    threshold = i;
  }

  fill_mask(matrix, size, threshold, mask);

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%d ", mask[i][j]);
    }
    printf("\n");
  }
}