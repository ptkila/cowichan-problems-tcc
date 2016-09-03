#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;
static int n_threads;

void randmat (const int size, const int seed) {

  int i, j, s;

  #pragma omp parallel shared(matrix, seed, size) private (i, j, s)
  {
    #pragma omp for schedule (static, size/n_threads)
    for (i = 0; i < size; ++i) {
      for (j = 0; j < size; ++j) {
        s = VAL_A * (seed + i + j) + VAL_B;
        matrix[i* size + j] = s % 100;
      }
    }
  }
} 

void set_threads_number() {

  omp_set_num_threads(n_threads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof (int) * size * size);

    set_threads_number();
    randmat(size, rand());

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%d ", matrix[i * size + j]);
        }
        printf("\n");
      }
    }

    free(matrix);

  } else {


  }

  return 0;
}