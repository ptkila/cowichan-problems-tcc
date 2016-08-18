#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;

void randmat (const int size, const int seed) {

  int i, j, s;
  int nt = omp_get_num_threads();

  #pragma omp parallel shared(matrix, seed, size, s)
  {
    #pragma omp for schedule (static, size/ nt)
    for (i = 0; i < size; ++i) {
      for (j = 0; j < size; ++j) {
        s = VAL_A * (seed + i + j) + VAL_B;
        matrix[i* size + j] = s % 100;
      }
    }
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

    set_threads_number(num_threads);
    randmat(size, rand());

    if (print == 1) {
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%d ", matrix[i * size + j]);
        }
        printf("\n");
      }
    }
  } else {


  }

  return 0;
}