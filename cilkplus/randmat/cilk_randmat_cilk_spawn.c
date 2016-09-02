#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const int VAL_A = 1313, VAL_B = 3131;
static int *matrix;
static int n_threads;

void fill_matrix(const int begin, const int end, const int size, const int seed) {
  if (begin + 1 == end) {
    
    int i, s;
    for (i = 0; i < size; i++) {
      s = VAL_A * (seed + i) + VAL_B;
      matrix[begin*size + i] = s % 100;
    }

    return;
    
  } else {
  
    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_matrix(begin, middle, size, seed);
    cilk_spawn fill_matrix(middle, end, size, seed);
    cilk_sync;

  }
}

void randmat(const int size, const int seed) {

  cilk_spawn fill_matrix(0, size, size, seed);
  cilk_sync;

}

void set_threads_number () {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

}

int main(int argc, char *argv[]) {
  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof(int) * size * size);
    
    set_threads_number();
    
    cilk_spawn randmat(size, rand());
    cilk_sync;

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%d ", matrix[i*size + j]);
        }
        printf("\n");
      }
    }

    free(matrix);

  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;

}