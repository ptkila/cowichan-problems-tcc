#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

static double* matrix;
static double* result;
static double* vector;

void product (const int size) {

  int i, j;
  double sum = 0.0;
  
  #pragma omp parallel shared(result, matrix, vector, size) \
    private (j) reduction(+:sum)
  {
    #pragma omp for schedule (static)
    for (i = 0; i < size; ++i) {
      for (j = 0; j < size; ++j) {
        sum += matrix[i * size + j] * vector[j];
      }
      result[i] = sum;
    }
  }
}

void set_values_matrix(const int size) {

  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i * size + j] = rand() % 10;    
    }
  } 
}

void set_values_vector (const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    vector[i] = rand() % 10;
  }
}

void set_threads_number(const int n_threads) {

  omp_set_num_threads(n_threads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (double*) malloc (sizeof (double) * size * size);
    result = (double*) malloc (sizeof (double) * size);
    vector = (double*) malloc (sizeof (double) * size);

    set_threads_number(n_threads);
    set_values_matrix(size);
    set_values_vector(size);
    product(size);

    if (print == 1) {
      int i;
      for (i = 0; i < size; ++i) {
        printf("%.0f ", result[i]);
      }
      printf("\n");
    }

    free(matrix);
    free(result);
    free(vector);

  } else {

  }

  return 0;
}