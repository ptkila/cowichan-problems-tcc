#include "serial.h"

int set_max_number (int** matrix, int size) {

  int i, j;
  int tmpNumber = 0;
  
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (tmpNumber < matrix[i][j]) tmpNumber = matrix[i][j];
    }
  }
  return tmpNumber;
}

void set_histogram (int* histogram, int** matrix, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      histogram[matrix[i][j]]++;
    }
  }
}

int set_threshold (int* histogram, int max_number, int proportion) {

  int i, tmpNumber, prefix_sum = 0;

  for (i = max_number; i >= 0 && prefix_sum <= proportion; i--) {
    prefix_sum += histogram[i];
    tmpNumber = i;
  }

  return tmpNumber;
}

void set_mask (int** mask, int** matrix, int size, int threshold) {

  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i][j] = (int)(matrix[i][j] > threshold);
    }
  }
}

int** thresh (int** matrix, int size, int percent, int** mask) {

  int max_number, threshold, prefix_sum, proportion, i, j;
  int* histogram;

  max_number = set_max_number(matrix, size);
  
  histogram = (int*)malloc(sizeof(int) * (max_number + 1));
  set_histogram(histogram, matrix, size);

  proportion = (size * size * percent) / 100;
  threshold = set_threshold(histogram, max_number, proportion);
  set_mask(mask, matrix, size, threshold);
  
}