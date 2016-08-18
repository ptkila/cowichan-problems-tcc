#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const int VAL_A = 1313, VAL_B = 3131;
static int *matrix;

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

void randmat(const int size, const int seed) {
  int i, j, s;

  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      s = VAL_A * (seed + i + j) + VAL_B;
      matrix[i*size + j] = s % 100;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    int s, i, j;
    s = rand();
    matrix = (int*) malloc (sizeof(int)*size*size);

    set_threads_number(num_threads);
    randmat(size, s);

    if (print == 1) {
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