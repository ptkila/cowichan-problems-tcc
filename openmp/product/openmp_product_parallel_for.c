#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

static int* matrix;
static int* result;
static int* vector;

void product (const int size) {

  int i, j;

  #pragma omp parallel shared(result, i, j)
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        result[i * size + j] += matrix[i * size + j] * vector[j];
      }
    }
  }
}

void set_values_matrix(const int size) {

  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i * size + j] = rand() % 100;    
    }
  }
}

void set_values_vector (const int size) {

  int i;
  for (i = 0; i < size; ++i) {
    vector[i] = rand() % 100;
  }
}

void set_threads_number(const int threads_number) {

  omp_set_num_threads(threads_number);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int i, j;

    matrix = (int*) malloc (sizeof (int) * size * size);
    result = (int*) malloc (sizeof (int) * size * size);
    vector = (int*) malloc (sizeof (int) * size);

    set_values_matrix(size);
    set_values_vector(size);
    set_threads_number(num_threads);
    product(size);

    if (print == 1) {
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%d ", result[i * size + j]);
        }
        printf("\n");
      }
    }
  } else {


  }

  return 0;
}