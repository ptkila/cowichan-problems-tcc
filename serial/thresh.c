#include "serial.h"

void thresh (int** matrix, int size, int percent, int** mask) {

  int max_number = 0;
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (max_number < matrix[i][j])
        max_number = matrix[i][j];
    }
  }

  int* histogram = (int*)malloc(sizeof(int) * (max_number + 1));

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      histogram[matrix[i][j]]++;
    }
  }

  int threshold = max_number;
  int prefix_sum = 0;
  int count = (size * size * percent) / 100;

  for (i = max_number; i >= 0 && prefix_sum <= count; i--) {
    prefix_sum += histogram[i];
    threshold = i;
  }

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i][j] = (int)(matrix[i][j] >= threshold);
    }
  }

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%d ", mask[i][j]);
    }
    printf("\n");
  }

}