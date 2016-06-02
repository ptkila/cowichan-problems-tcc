#include "cilkplus.h"

double sqr (double x) {

  return x * x;

}

double distance(struct point point_one, struct point point_two) {

  // distncia euclidiana
  return sqrt(sqr(point_one.x - point_one.y) + sqr(point_two.x - point_two.y));

}

struct outer_struct outer (struct point *points, int number_of_points, int num_threads, int should_print) {

  int i = 0, j = 0;
  double **matrix;
  double *vector;
  double n_max = -1.0;
  struct point origin;
  struct outer_struct result;

  matrix = (double**)malloc(number_of_points * sizeof(double*));
  for (i = 0; i < number_of_points; i++) {
    matrix[i] = (double*)malloc(number_of_points * sizeof(double));
  }

  vector = (double*)malloc(number_of_points * sizeof(double));

  origin.x = 0;
  origin.y = 0;

  cilk_for (i = 0; i < number_of_points; i++) {
    for (j = 0; j < number_of_points; j++) {
      if (i != j) {
        matrix[i][j] = distance(points[i], points[j]);
        n_max = fmax(n_max, matrix[i][j]);
      }
    }
    matrix[i][i] = n_max * number_of_points;
    vector[i] = distance(origin, points[i]);
  }

  if (should_print == 1)
  {
    for (i = 0; i < number_of_points; i++) {
      for (j = 0; j < number_of_points; j++) {
        printf("%e ", matrix[i][j]);
      }
      printf("\n");
    }

    for (i = 0; i < number_of_points; i++) {
      printf("%e ", vector[i]);
    }

    printf("\n");
  }

  result.matrix = matrix;
  result.vector = vector;
  return result;

}