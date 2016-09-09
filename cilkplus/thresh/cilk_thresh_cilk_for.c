#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int *matrix;
static int *mask;
static int *histogram;
static int n_threads;

int find_max(const int begin, const int end, const int size) {
  int left, right;
  
  if (begin + 1 == end) {
    
    int i;
    int res = matrix[begin*size + 0];
    for (i = 1; i < size; ++i) {
      res = MAX(res, matrix[begin*size + i]);
    }
    return res;

  } else {
    
    int middle = begin + (end - begin) / 2;
    left = cilk_spawn find_max(begin, middle, size);
    right = cilk_spawn find_max(middle, end, size);
    cilk_sync;
    return MAX(left, right);
  
  }
}

void fill_histogram(const int size) {
  int i, j;
  cilk_for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      ++histogram[matrix[i*size + j]];
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

  nmax = find_max(size);

  fill_histogram(0, size, size);
  count = (size * size * percent)/ 100;

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
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 255;
    }
  }
}

void set_threads_number () {

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
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    histogram = (int*) malloc (sizeof(int) * 256);

    set_threads_number();
    set_values_matrix(size);
    thresh(size, percent);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
          printf("%hhu ", mask[i*size + j]);
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