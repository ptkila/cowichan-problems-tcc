#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include "omp.h"

struct point {
  double x;
  double y;
};
static struct point* norm_points;
static struct point* points;
static struct point min_point;
static struct point max_point;

void find_min_max_points (int size) {

  int i;

  max_point.x = points[0].x;
  max_point.y = points[0].y;
  min_point.x = points[0].x;
  min_point.y = points[0].y;

  #pragma omp parallel for default(none) shared(size, points, min_point, max_point)
  for (i = 1; i < size; i++) {

    if (points[i].x < min_point.x) {
      min_point.x = points[i].x;
    }

    if (points[i].y < min_point.y) {
      min_point.y = points[i].y;
    }

    if (points[i].x > max_point.x) {
      max_point.x = points[i].x;
    }

    if (points[i].y > max_point.y) {
      max_point.y = points[i].y;
    }
  }
}

void normalize_points (int size) {

  double sclX = 0, sclY = 0;
  int i = 0;

  // x = (xi - xmin) * (1 / (xmax - xmin))
  sclX = (double)((max_point.x == min_point.x) ?
    0.0 : 1.0 / (max_point.x - min_point.x));

  sclY = (double)((max_point.y == min_point.y) ?
    0.0 : 1.0 / (max_point.y - min_point.y));

  #pragma omp parallel shared(size, points, min_point, max_point, sclX, sclY, norm_points)
  {
    #pragma omp for
    for (i = 0; i < size; i++) {
      norm_points[i].x = sclX * (points[i].x - min_point.x);
      norm_points[i].y = sclY * (points[i].y - min_point.y);
    }
  }
}

void norm (int size) {

  find_min_max_points(size);
  normalize_points(size);

} 

void set_points_values(int size) {
  int i;
  for (i = 0; i < size; ++i) {
    points[i].x = (double)(rand());
    points[i].y = (double)(rand());
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
    int i, j;

    points = (struct point*) malloc(sizeof(struct point) * size);
    norm_points = (struct point*) malloc(sizeof(struct point) * size);

    set_points_values(size);
    set_threads_number(num_threads);
    norm(size);

    if (print == 1) {
      for (i = 0; i < size; i++) {
        printf("%f ", norm_points[i].x);
        printf("%f\n", norm_points[i].y);
      }
    }
  } else {


  }

  return 0;
}