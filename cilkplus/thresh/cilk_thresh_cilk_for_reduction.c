#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
#include <cilk/cilk_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int *matrix;
static int *mask;
static int *histogram;

int find_max (const int size) {
  int i, j;

  CILK_C_REDUCER_MAX(r, int, 0);
  CILK_C_REGISTER_REDUCER(r);
  cilk_for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      CILK_C_REDUCER_MAX_CALC(r, matrix[i*size + j]);
    }
  }
  int max = REDUCER_VIEW(r);
  CILK_C_UNREGISTER_REDUCER(r);

  return max;
}

void fill_histogram(const int size) {
  int i, j;
  cilk_for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      ++histogram[matrix[i*size + j]];
    }
  }
  /*
  printf("\n");
  for (int i = 0; i < 256; ++i) {
    printf("%d ", histogram[i]);
  }
  printf("\n");
  */
}

void fill_mask (const int size, const int threshold) {
  int i, j;
  cilk_for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      mask[i*size + j] = (matrix[i*size + j] >= threshold);
    }
  }
}

int calc_threshold (const int percent, const int nmax, const int size) {
  int i;
  int count = (size * size * percent)/ 100;
  int prefixsum = 0;
  int threshold = nmax;

  for (i = nmax; i >= 0 && prefixsum <= count; --i) {
    prefixsum += histogram[i];
    threshold = i;
  }

  return threshold;
}

void thresh(const int size, const int percent) {
  
  int nmax = find_max(size);

  //printf("%d\n", nmax);
  
  fill_histogram(size);
  
  int threshold = calc_threshold(percent, nmax, size);
  
  fill_mask(size, threshold);

}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 255;
    }
  }
/*
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      printf("%d ", matrix[i*size + j]);
    }
    printf("\n");
  }
  printf("\n");
  */
}

void set_threads_number (const int n_threads) {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char *argv[]) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    histogram = (int*) malloc (sizeof(int) * 256);

    set_threads_number(n_threads);
    set_values_matrix(size);
    thresh(size, percent);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
          printf("%d ", mask[i*size + j]);
        }
        printf("\n");
      }
    }

    free(matrix);
    free(mask);
    free(histogram);

  } else {


  }

  return 0;
}