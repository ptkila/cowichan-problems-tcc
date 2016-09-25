#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX( a, b ) ( ( a > b) ? a : b )

static int *matrix;
static int *mask;
static int *histogram;

int find_max(const int size) {
  
  int i, j;
  int res = 0;

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      res = MAX(res, matrix[i*size + j]);
    }
  }

  return res;

}

void fill_histogram(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      ++histogram[matrix[i*size + j]];
    }
  }
}

void fill_mask (const int size, const int threshold) {
  int i, j;
  for (i = 0; i < size; ++i) {
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
  
  int nmax = find_max(size);
  
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

int main(int argc, char *argv[]) {

  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);
    int percent = 50;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    histogram = (int*) malloc (sizeof(int) * 256);

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

     printf("programa <tamanho> <printar>\n");

  }

  return 0;
}