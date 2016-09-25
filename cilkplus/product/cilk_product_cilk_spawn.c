#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static double* matrix;
static double* vector;
static double* result;
static int n_threads;

void fill_result(const int begin, const int end, const int size) {
  
  if (begin + 1 == end) {

    int i;
    double sum = 0.0;
    for (i = 0; i < size; ++i) {
      sum += matrix[begin*size + i] * vector[i];
    }
    result[begin] = sum;
    
    return;

  } else {

    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_result(begin, middle, size);
    cilk_spawn fill_result(middle, end, size);
    cilk_sync;
  
  }
}

void product(const int size) {

  cilk_spawn fill_result(0, size, size);
  cilk_sync;

}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = (float)rand();
    }
  }
}

void set_values_vector(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    vector[i] = (float)rand();
  }
}

void set_threads_number () {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

}

int main(int argc, char** argv) {

  if (argc == 4) {
    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (double*) malloc (sizeof(double) * size * size);
    result = (double*) calloc (sizeof(double), size);
    vector = (double*) malloc (sizeof(double) * size);

    set_threads_number();
    set_values_matrix(size);
    set_values_vector(size);

    cilk_spawn product(size);
    cilk_sync;

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

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;
}