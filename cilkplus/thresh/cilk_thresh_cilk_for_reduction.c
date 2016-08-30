#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int *matrix;
static int *mask;
static int *histogram;

int reduce_max (const int size) {
  int i, j;
  int max = 0;
  CILK_C_REDUCER_MAX(r, int, 0);
  CILK_C_REGISTER_REDUCER(r);

  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      CILK_C_REDUCER_MAX_CALC(r, matrix[i*size + j]);
    }
  }
  max = r.value;
  CILK_C_UNREGISTER_REDUCER(r);

  return max;
}

void fill_histogram(const int size) {
  int i, j;
  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      histogram[matrix[i*size + j]]++;
    }
  }
}

void fill_mask (const int size, const int threshold) {
  int i, j;
  cilk_for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      mask[i*size + j] = matrix [i*size + j] >= threshold;
    }
  }
}

void thresh(const int size, const int percent) {
  int i;
  int nmax = 0;
  int count, prefixsum, threshold;

  nmax = reduce_max(size);

  fill_histogram(size);
  count = (size * size * percent) / 100;

  prefixsum = 0;
  threshold = nmax;

  for (i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[i];
    threshold = i;
  }

  fill_mask(size, threshold);
}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size + j] = rand() % 255;
    }
  }
}

void set_threads_number (const int t_num) {

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
    int percent = 50;
    int i, j;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    histogram = (int*) malloc (sizeof(int) * 256);

    set_threads_number(num_threads);
    set_values_matrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%hhu ", mask[i*size + j]);
        }
        printf("\n");
      }
    }

  } else {


  }

  return 0;
}