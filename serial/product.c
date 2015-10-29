#include "serial.h"

void product (double **matrix, double *vector, int size) {
  int i = 0;
  int j = 0;
  double** result;

  result = (double**)malloc(sizeof(double*) * size);
  for (i = 0; i < size; i++) {
    result[i] = (double*)malloc(sizeof(double) * size);
  }

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      result[i][j] += matrix[i][j] * vector[j];
    }
  }

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%f ", result[i][j] );
    }
    printf("\n");
  }
}