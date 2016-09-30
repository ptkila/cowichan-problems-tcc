#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX( a, b ) ( ( a > b) ? a : b )

static int *matrix;
static int *mask;
static int *histogram;

int find_max(const int begin, const int end, const int size) {
  
  if (begin + 1 == end) {
    
    int i;
    CILK_C_REDUCER_MAX(r, int, 0);
    CILK_C_REGISTER_REDUCER(r);
    cilk_for (i = 0; i < size; ++i) {
      CILK_C_REDUCER_MAX_CALC(r, matrix[begin*size + i]);
    }
    int max = REDUCER_VIEW(r);
    CILK_C_UNREGISTER_REDUCER(r);

    return max;

  } else {

    int middle = begin + (end - begin) / 2;
    int left = cilk_spawn find_max(begin, middle, size);
    int right = cilk_spawn find_max(middle, end, size);
    cilk_sync;
    return MAX(left, right);
  
  }
}

void fill_histogram(const int begin, const int end, const int size) {

  if (begin + 1 == end) {
    int i;
    for (i = 0; i < size; ++i) {
      ++histogram[matrix[begin*size + i]];
    }

    return;
  
  } else {
    
    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_histogram(begin, middle, size);
    cilk_spawn fill_histogram(middle, end, size);
    cilk_sync;
  
  }
}

void fill_mask(const int begin, const int end, const int size, const int threshold) {
  if (begin + 1 == end) {

    int i;
    for (i = 0; i < size; ++i) {
      mask[begin*size + i] = matrix[begin*size + i] >= threshold;
    }
    return;
  
  } else {
  
    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_mask(begin, middle, size, threshold);
    cilk_spawn fill_mask(middle, end, size, threshold);
    cilk_sync;
  
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

  int nmax = cilk_spawn find_max(0, size, size);
  cilk_sync;

  cilk_spawn fill_histogram(0, size, size);
  cilk_sync;

  int threshold = calc_threshold(percent, nmax, size);

  cilk_spawn fill_mask(0, size, size, threshold);
  cilk_sync;

}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 255;
    }
  }
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

    cilk_spawn thresh(size, percent);
    cilk_sync;

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