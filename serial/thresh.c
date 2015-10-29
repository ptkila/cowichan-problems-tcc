#include "serial.h"

void thresh (int** matrix, int size, int percent, int** mask) {

  int maxNumber = 0;
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (maxNumber < matrix[i][j])
        maxNumber = matrix[i][j];
    }
  }

  int* histogram = (int*)malloc(sizeof(int) * (maxNumber + 1));

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      histogram[matrix[i][j]]++;
    }
  }

  int threshold = maxNumber;
  int prefixSum = 0;
  int count = (size * size * percent) / 100;

  for (i = maxNumber; i >= 0 && prefixSum <= count; i--) {
    prefixSum += histogram[i];
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