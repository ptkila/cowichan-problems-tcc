#include "openmp.h"

int reduce_max (int** matrix, int size) {

  int i, j;
  int max_val = 0;

  #pragma omp parallel for reduction(max : max_val)
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (max_val < matrix[i][j])
        max_val = matrix[i][j];
    }
  }
  return max_val;
}

void fill_histogram(int** matrix, int size, int* histogram) {
  
  int i, j;

  #pragma omp parallel shared(matrix, histogram, i, j)
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        histogram[matrix[i][j]]++;
      }
    }
  }
}

void fill_mask(int** matrix, int size, int threshold, int** mask) {
  
  int i, j;
  
  #pragma omp parallel
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        mask[i][j] = (int)(matrix[i][j] >= threshold);
      }
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
  int prefix_sum = 0;
  int threshold = n_max;

  for (i = n_max; i >= 0 && prefix_sum <= count; i--) {
    prefix_sum += histogram[i];
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