#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static double* matrix;
static double* vector;
static double* result;

void product (const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    double sum = 0.0;
    for (j = 0; j < size; ++j) {
      sum += matrix[i*size + j] * vector [j];
    }
    result[i] = sum;
  }
}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = (float) rand();
    }
  }
}

void set_values_vector(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    vector[i] = (float) rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);

    matrix = (double*) malloc (sizeof(double) * size * size);
    result = (double*) calloc (sizeof(double), size);
    vector = (double*) malloc (sizeof(double) * size);

    set_values_matrix(size);
    set_values_vector(size);
    product(size);

    if (print == 1) {
      int i;
      for (i = 0; i < size; ++i) {
        printf("%g ", result[i]);
      }
      printf("\n");
    }

    free(matrix);
    free(vector);
    free(result);

  } else {

    printf("programa <tamanho> <printar>\n");

  }

  return 0;
}