#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <limits.h>
#include "omp.h"

struct point_w {
  int weight;
  int x;
  int y;
};

static int nelts;
static int* matrix;
static int* mask;
static struct point_w* vector;
static int n_threads;

void fill_w_vector (const int size) {
  int i, j;
  int counter = 0;

  #pragma omp parallel shared(matrix, vector, mask, counter) private (i, j)
  #pragma omp for schedule (static, size/ n_threads)
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      #pragma omp critical
      {
        if(mask[i*size + j] == 1) {  
          vector[counter].weight = matrix[i*size + j];
          vector[counter].x = i;
          vector[counter].y = j;
          counter++;
        }
      }
    }
  }
}

int compare (const void * a, const void * b) {

  struct point_w* point0 = (struct point_w*) a;
  struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void winnow (const int size) {

  fill_w_vector(size);
  qsort(vector, nelts, sizeof(struct point_w), compare);

}

void set_values_matrix(const int size) {
  int i, j;
  for (i =  0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size + j] = rand();
    }
  }
}

void set_values_mask(const int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i*size + j] = rand() % 2;
      if(mask[i*size + j]) { 
        nelts++;
      }
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

    matrix = (int*) malloc (sizeof (int) * size * size);
    mask = (int*) malloc (sizeof (int) * size * size);
    nelts = 0;

    set_threads_number();
    set_values_matrix(size);
    set_values_mask(size);

    vector = (struct point_w*) malloc(sizeof(struct point_w) * nelts);

    winnow(size);

    if (print == 1) {
      int i;
      for (i = 0; i < nelts; i++) {
        printf("%d %d %d", vector[i].x, vector[i].y, vector[i].weight);
        printf("\n");
      }
    }

    free(matrix);
    free(mask);
    free(vector);

  } else {


  }

  return 0;
}