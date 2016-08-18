#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

static int* matrix;
static int* mask;
static int* histogram; 

int reduce_max (int size) {

  int i, j;
  int max_val = 0;

  #pragma omp parallel for reduction(max : max_val)
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (max_val < matrix[i * size + j])
        max_val = matrix[i * size + j];
    }
  }
  return max_val;
}

void fill_histogram(int size) {
  
  int i, j;

  #pragma omp parallel shared(matrix, histogram, i, j)
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        histogram[matrix[i* size + j]]++;
      }
    }
  }
}

void fill_mask(int size, int threshold) {
  
  int i, j;
  
  #pragma omp parallel
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        mask[i* size + j] = (int)(matrix[i* size + j] > threshold);
      }
    }
  }
}

for(int j = 0; j < n; j++){
    aux = pow(x[j], 2) + pow(y[j], 2);
    if(aux <= 1){
      k++;
      X[k] = x[j] * (sqrt((-2 * log(aux)) / aux));
      Y[k] = y[j] * (sqrt((-2 * log(aux)) / aux));
    }
  }

void thresh(int size, int percent) {
  
  int i, j;
  int n_max = 0;

  n_max = reduce_max(size);

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

void set_values_matrix(int size) {

  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 255;    
    }
  }
}

void set_threads_number(int threads_number) {

    omp_set_num_threads(threads_number);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = (int*) malloc (sizeof (int) * size * size);
    mask = (int*) malloc (sizeof (int) * size * size);
    histogram = (int*) calloc (sizeof (int), 255);

    set_values_matrix(size);
    set_threads_number(num_threads);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%hhu ", mask[i*size + j]);
        }
        printf("\n");
      }
    }
  } else {


  }

  return 0;
}