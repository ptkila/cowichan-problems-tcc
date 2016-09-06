#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <limits.h>
#include "omp.h"

struct point_w {
  int weight, i, j;
};

static int* matrix;
static int* mask;
static int* offsets;
static struct point_w* ev_values;
static struct point_w* points;
static int nelts;
static int n_threads;

int count_points (const int size) {
  
  int i, j;
  int counter = 0;
  int sum = 0;

  #pragma omp parallel shared(mask) private(i, j) firstprivate(sum)
  {
    int thread_num = omp_get_thread_num();
    #pragma omp for schedule(static, size/ n_threads)
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        if (mask[i*size + j]) {
          sum++;
        }
      }
    }
    offsets[thread_num] = sum;
  }
  
  int len = 0;
  for (i = 0; i < n_threads; i++) {
    int tmp = offsets[i];
    offsets[i] = len;
    len += tmp;
  }

  return len;

}

void fill_values(const int size) {

  int i, j;
  int off = 0;

  #pragma omp parallel shared(mask, matrix) private(off, i, j)
  {
    int thread_num = omp_get_thread_num();
    off = offsets[thread_num];
    #pragma omp for schedule(static, size/ n_threads)
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        if (mask[i*size + j]) {
          ev_values[off].weight = matrix[i*size + j];
          ev_values[off].i = i;
          ev_values[off].j = j;
          off++;
        }
      }
    }
  }
}

void fill_ev_points(const int len) {
  int i;
  int chunk = (int)floor((float)len/ (float)nelts);
  for(i = 0; i < nelts; i++) {
    points[i] = ev_values[i*chunk];
  }
}

int compare (const void * a, const void * b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void winnow (const int size) {

  int len = count_points(size);
  ev_values = (struct point_w*) malloc(sizeof(struct point_w) * len);
  
  fill_values(size);
  qsort(ev_values, len, sizeof(*ev_values), compare);

  nelts = rand() % len;
  if (nelts == 0) {
    nelts = 1;
  }

  points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);
  fill_ev_points(len);

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
    offsets = (int*) malloc (sizeof (int) * n_threads);
    nelts = 0;

    set_threads_number();
    set_values_matrix(size);
    set_values_mask(size);

    winnow(size);

    if (print == 1) {
      int i;
      for (i = 0; i < nelts; i++) {
        printf("%d %d %d", ev_values[i].i, ev_values[i].j, ev_values[i].weight);
        printf("\n");
      }
    }

    free(matrix);
    free(mask);
    free(offsets);
    free(ev_values);
    free(points);

  } else {


  }

  return 0;
}