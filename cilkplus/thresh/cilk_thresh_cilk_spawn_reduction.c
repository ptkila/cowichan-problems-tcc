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

int max(int a, int b) {
  return a > b ? a : b;
}

int reduce_max(int begin, int end, int ncols) {
  int middle = begin + (end - begin) / 2;
  int left, right, res, i;
  if (begin + 1 == end) {
    res = matrix[begin*ncols];
    for (i = 1; i < ncols; i++) {
      res = max(res, matrix[begin*ncols + i]);
    }
    return res;
  }
  left = cilk_spawn reduce_max(begin, middle, ncols);
  right = cilk_spawn reduce_max(middle, end, ncols);
  cilk_sync;
  return max(left, right);
}

void fill_histogram(int begin, int end, int ncols) {
  int middle = begin + (end - begin) / 2;
  int i;

  if (begin + 1 == end) {
    for (i = 0; i < ncols; i++) {
      histogram[matrix[begin*ncols + i]]++;
    }
    return;
  }
  cilk_spawn fill_histogram(begin, middle, ncols);
  cilk_spawn fill_histogram(middle, end, ncols);
  cilk_sync;
}

void fill_mask(int begin, int end, int ncols, int threshold) {
  int middle = begin + (end - begin) / 2;
  int i;
  if (begin + 1 == end) {
    for (i = 0; i < ncols; i++) {
      mask[begin*ncols + i] = matrix[begin*ncols + i] >= threshold;
    }
    return;
  }
  cilk_spawn fill_mask(begin, middle, ncols, threshold);
  cilk_spawn fill_mask(middle, end, ncols, threshold);
  cilk_sync;
}

void thresh(int nrows, int ncols, int percent) {
  int i;
  int nmax = 0;
  int count, prefixsum, threshold;

  nmax = cilk_spawn reduce_max(0, nrows, ncols);
  cilk_sync;

  cilk_spawn fill_histogram(0, nrows, ncols);
  cilk_sync;

  count = (nrows * ncols * percent) / 100;

  prefixsum = 0;
  threshold = nmax;

  for (i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[i];
    threshold = i;
  }

  cilk_spawn fill_mask(0, nrows, ncols, threshold);
  cilk_sync;
}

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

void set_values_matrix(int nrows, int ncols) {
  
  int i, j;

  for (i = 0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      matrix[i*ncols + j] = rand() % 255;
    }
  }
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
    histogram =(int*) malloc (sizeof(int) * 255);

    set_values_matrix(size, size);
    set_threads_number(num_threads);
    cilk_spawn thresh(size, size, percent);
    cilk_sync;

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