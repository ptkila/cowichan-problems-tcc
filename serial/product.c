#include "serial.h"

void calculate_product(double** matrix, double* vector, double** result, int size) {

  int i, j;
  
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      result[i][j] += matrix[i][j] * vector[j];
    }
  }
}

double** product (double **matrix, double *vector, int size) {

  int i;
  double** result;

  // Set initial vars
  result = (double**)malloc(sizeof(double*) * size);
  for (i = 0; i < size; i++) {
    result[i] = (double*)malloc(sizeof(double) * size);
  }
  
  calculate_product(matrix, vector, result, size);

  return result;

}