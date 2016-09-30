#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
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
    int max = -1;
    for (i = 0; i < size; ++i) {
      max = MAX(max, matrix[begin*size + i]);
    }
    return max;

  } else {
    
    int middle = begin + (end - begin) / 2;
    int left = cilk_spawn find_max(begin, middle, size);
    int right = cilk_spawn find_max(middle, end, size);
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
  
  int nmax = find_max(0, size, size);
  
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