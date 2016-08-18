#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <limits.h>
#include "omp.h"

struct point {
  double x;
  double y;
};

static double* matrix;
static double* vector;
static struct point* points;

double sqr (const double x) {
  return x * x;
}

double distance(const struct point a, const struct point b) {
  return sqrt(sqr(a.x - a.y) + sqr(b.x - b.y));
}

void outer (const int size) {

  int i, j;
  double n_max = -1.0;
  struct point origin;

  origin.x = 0;
  origin.y = 0;

  #pragma omp parallel shared(matrix, i, j)
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
        if (i != j) {
          matrix[i * size + j] = distance(points[i], points[j]);
          n_max = fmax(n_max, matrix[i* size + j]);
        }
      }
      matrix[i* size + i] = n_max * size;
      vector[i] = distance(origin, points[i]);
    }
  }
}

void set_points_values(const int size) {
  int i;
  for (i = 0; i < size; i++) {
    points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
    points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
  }
}

void set_threads_number(const int threads_number) {

  omp_set_num_threads(threads_number);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (double*) malloc (sizeof (double) * size * size);
    vector = (double*) malloc (sizeof (double) * size);
    points = (struct point*) malloc(sizeof (struct point) * size);

    set_points_values(size);
    set_threads_number(num_threads);
    outer(size);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%e ", matrix[i* size + j]);
        }
        printf("\n");
      }

      printf("\n");

      for (i = 0; i < size; i++) {
        printf("%e ", vector[i]);
      }

      printf("\n");

    }
  } else {


  }

  return 0;
}