#include "serial.h"

double sqr (double x) {

  return x * x;

}

double distance(struct point point_one, struct point point_two) {

  // distncia euclidiana
  return sqrt(sqr(point_one.x - point_one.y) + sqr(point_two.x - point_two.y));

}

void outer (struct point *points, int number_of_points) {
  int i = 0, j = 0;
  double **matrix = (double**)malloc(number_of_points * sizeof(double*));
  double *vector = (double*)malloc(number_of_points * sizeof(double));
  double n_max = -1.0;
  struct point origin;

  origin.x = 0;
  origin.y = 0;

  for (i = 0; i < number_of_points; i++) {
    matrix[i] = (double*)malloc(number_of_points * sizeof(double));
  }

  for (i = 0; i < number_of_points; i++) {
    for (j = 0; j < number_of_points; j++) {
      if (i != j) {
        matrix[i][j] = distance(points[i], points[j]);
        n_max = fmax(n_max, matrix[i][j]);
      }
    }
    matrix[i][i] = n_max * number_of_points;
    vector[i] = distance(origin, points[i]);
  }

  for (i = 0; i < number_of_points; i++) {
    for (j = 0; j < number_of_points; j++) {
      printf("%1.lf ", matrix[i][j]);
    }
    printf("\n");
  }

  for (i = 0; i < number_of_points; i++) {
    printf("%1.lf ", vector[i]);
  }
   
  printf("\n");
}