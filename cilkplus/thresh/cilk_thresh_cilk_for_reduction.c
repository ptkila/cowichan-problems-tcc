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

int reduce_max (int nrows, int ncols) {
  int i, j;
  int max = 0;
  CILK_C_REDUCER_MAX(r, int, 0);
  CILK_C_REGISTER_REDUCER(r);

  cilk_for (i = 0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      CILK_C_REDUCER_MAX_CALC(r, matrix[i*ncols + j]);
    }
  }
  max = r.value;
  CILK_C_UNREGISTER_REDUCER(r);

  return max;
}

void fill_histogram(int nrows, int ncols) {
  int i, j;
  cilk_for (i = 0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      histogram[matrix[i*ncols + j]]++;
    }
  }
}

void fill_mask (int nrows, int ncols, int threshold) {
  cilk_for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      mask[i*ncols + j] = matrix [i*ncols + j] >= threshold;
    }
  }
}

void thresh(int nrows, int ncols, int percent) {
  int i;
  int nmax = 0;
  int count, prefixsum, threshold;

  nmax = reduce_max(nrows, ncols);

  fill_histogram(nrows, ncols);
  count = (nrows * ncols * percent) / 100;

  prefixsum = 0;
  threshold = nmax;

  for (i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[i];
    threshold = i;
  }

  fill_mask(nrows, ncols, threshold);
}

void set_values_matrix(int nrows, int ncols) {
  
  int i, j;

  for (i = 0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      matrix[i*ncols + j] = rand() % 255;
    }
  }
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
    int percent = 50;
    int i, j;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    histogram = (int*) malloc (sizeof(int) * 255);

    set_values_matrix(size, size);
    set_threads_number(num_threads);
    thresh(size, size, percent);

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