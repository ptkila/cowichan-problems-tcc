#include "serial.h"

double sqr (double x) {

  return x * x;

}

double distance(struct point point_one, struct point point_two) {

  return sqrt(sqr(point_one.x - point_one.y) + sqr(point_two.x - point_two.y));

}

void calculate_points(double **matrix, double *vector, struct point* points, struct point origin, int number_of_points) {

  int i = 0, j = 0;
  double n_max = -1;

  for (i = 0; i < number_of_points; ++i) {
    for (j = 0; j < number_of_points; ++j) {
      if (i != j) {
        matrix[i][j] = distance(points[i], points[j]);
        n_max = fmax(n_max, matrix[i][j]);
      }
    }
    matrix[i][i] = n_max * number_of_points;
    vector[i] = distance(origin, points[i]);
  }
}

void set_output(double** matrix, double* vector, struct outer_return out) {

  out.matrix = matrix;
  out.vector = vector;

}

struct outer_return outer (struct point *points, int number_of_points) {
  
  double **matrix, *vector, n_max;
  struct point origin; 
  struct outer_return out;
  int i;

  //Set initial vars
  matrix = (double**)malloc(number_of_points * sizeof(double*));
  for (i = 0; i < number_of_points; i++) {
    matrix[i] = (double*)malloc(number_of_points * sizeof(double));
  }
  vector = (double*)malloc(number_of_points * sizeof(double));
  origin.x = origin.y = 0;

  calculate_points(matrix, vector, points, origin, number_of_points);
  set_output(matrix, vector, out);

  return out;
}