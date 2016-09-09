#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "omp.h"

struct point {
  double x;
  double y;
};

static double* matrix;
static double* vector;
static struct point* points;
static struct point origin;
static int n_threads;

double sqr (const double x) {

  return x * x;

}

double distance(const struct point a, const struct point b) {

  return sqrt(sqr(a.x - a.y) + sqr(b.x - b.y));

}

void turn_to_matrix_and_vector (const int size) {

  int i, j;

  #pragma omp parallel shared(matrix, size) private (i, j)
  {
    #pragma omp for schedule(static, size/ n_threads)
    for (i = 0; i < size; ++i) {
      double n_max = 0.0;
      for (j = 0; j < size; ++j) {
        if (i != j) {
          matrix[i * size + j] = distance(points[i], points[j]);
          n_max = fmax(n_max, matrix[i* size + j]);
        }
      }
      matrix[i*size + i] = n_max*size;
      vector[i] = distance(origin, points[i]);
    }
  }

}

void outer (const int size) {

  turn_to_matrix_and_vector(size);
  
}

void set_points_values(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
    points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
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

    matrix = (double*) malloc (sizeof (double) * size * size);
    vector = (double*) malloc (sizeof (double) * size);
    points = (struct point*) malloc(sizeof (struct point) * size);

    set_points_values(size);
    set_threads_number();
    outer(size);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
          printf("%e ", matrix[i* size + j]);
        }
        printf("\n");
      }

      printf("\n");

      for (i = 0; i < size; ++i) {
        printf("%e ", vector[i]);
      }

      printf("\n");

    }

    free(matrix);
    free(vector);
    free(points);

  } else {


  }

  return 0;
}