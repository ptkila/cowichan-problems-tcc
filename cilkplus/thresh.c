#include "serial.h"

int reduce_max (int** matrix, int size) {
  CILK_C_DECLARE_REDUCER(int) max_reducer = CILK_C_INIT_REDUCER(int,
      add_int_reduce,
      add_int_identity,
      __cilkrts_hyperobject_noop_destroy,
      0);
  int i, j;
  int tmp_max = 0;

  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (tmp_max < matrix[i][j]) {
        tmp_max = matrix[i][j];
      }
    }
    max_reducer.calc_max (tmp_max);
  }
  return max_reducer.get_value ();
}

void fill_histogram(int** matrix, int size, int* histogram) {
  cilk_for (i = 0; i < size; ++i) {
    for (int j = 0; j < ncols; j++) {
      histogram[matrix[i][j]]++;
    }
  i
}

void merge_histogram () {
  int P = __cilkrts_get_nworkers();
  cilk_for (int v = 0; v < 100; ++v) {
    int merge_val = __sec_reduce_add (histogram [1:(P - 1)][v]);
    histogram [0][v] += merge_val;
  }
}

void fill_mask (int nrows, int ncols, int threshold) {
  cilk_for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      mask[i * ncols + j] = matrix [i * ncols + j] >= threshold;
    }
  }
}

void thresh(int nrows, int ncols, int percent) {
  int i;
  int nmax = 0;
  int count, prefixsum, threshold;

  nmax = reduce_max(nrows, ncols);

  fill_histogram(nrows, ncols);
  merge_histogram();

  count = (nrows * ncols * percent) / 100;

  prefixsum = 0;
  threshold = nmax;

  for (i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[0][i];
    threshold = i;
  }

  fill_mask(nrows, ncols, threshold);
}