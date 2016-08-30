#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

static int* matrix;
static int* mask;
static int* histogram; 
static int n_threads;

int find_max (const int size) {

  int i, j;
  int max_val = 0;

  #pragma omp parallel shared (matrix, size) private (i, j)
  {
    #pragma omp for schedule(static, size/n_threads)
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        #pragma omp critical
        {  
          if (max_val < matrix[i * size + j]) {
            max_val = matrix[i * size + j];
          }
        }
      }
    }
  }
  return max_val;
}

void fill_histogram(const int size) {

  int i, j;

  #pragma omp parallel shared(matrix, histogram) private(i, j)
  {
    #pragma omp for schedule (static, size/ n_threads)
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        histogram[matrix[i* size + j]]++;
      }
    }
  }
}

void fill_mask(const int size, const int threshold) {

  int i, j;
  
  #pragma omp parallel shared(mask, matrix, threshold) private (i, j)
  {
    #pragma omp for schedule (static, size/ n_threads)
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        mask[i* size + j] = (matrix[i* size + j] >= threshold);
      }
    }
  }
}

void thresh(const int size, const int percent) {

  int i, j;
  int n_max = 0;

  n_max = find_max(size);

  fill_histogram(size);

  int count = (size * size * percent) / 100;
  int prefix_sum = 0;
  int threshold = n_max;

  for (i = n_max; i >= 0 && prefix_sum <= count; i--) {
    prefix_sum += histogram[i];
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

void set_threads_number() {

  omp_set_num_threads(n_threads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = (int*) malloc (sizeof (int) * size * size);
    mask = (int*) malloc (sizeof (int) * size * size);
    histogram = (int*) calloc (sizeof (int), 256);

    set_values_matrix(size);
    set_threads_number();
    thresh(size, percent);

    if (print == 1) {
      int i, j;
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