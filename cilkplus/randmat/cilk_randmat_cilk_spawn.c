#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const int VAL_A = 1313, VAL_B = 3131;
static int *matrix;

void fill_row(const int begin, const int ncols, const int seed) {
  int i, s;
  for (i = 0; i < ncols; i++) {
    s = VAL_A * (seed + i) + VAL_B;
    matrix[begin * ncols + i] = s % 100;;
  }
}

void fill_matrix(const int begin, const int end, const int ncols, const int seed) {
  if (begin + 1 == end) {
    cilk_spawn fill_row(begin, ncols, seed);
  } else {
    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_matrix(begin, middle, ncols, seed);
    cilk_spawn fill_matrix(middle, end, ncols, seed);
  }
}

void randmat(int size, int seed) {

  cilk_spawn fill_matrix(0, size, size, s);

}

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char *argv[]) {
  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof(int)* size * size);
    set_threads_number(num_threads);
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
  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;

}