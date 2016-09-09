#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <float.h>
#include "omp.h"

struct point {
  double x;
  double y;
};

static struct point* norm_points;
static struct point* points;
static struct point min_point;
static struct point max_point;
static int n_threads;

void find_min_max_points (const int size) {

  int i = 0;

  double max_x = -DBL_MAX;
  double max_y = -DBL_MAX;
  double min_x = DBL_MAX;
  double min_y = DBL_MAX;
  
  #pragma omp parallel shared(size, points) private(i)
  {
    #pragma omp for schedule (static, size/ n_threads) reduction(max:max_x, max_y), reduction(min:min_x, min_y)
    for (i = 0; i < size; ++i) {
      if (points[i].x < min_x) {
        min_x = points[i].x;
      }

      if (points[i].y < min_y) {
        min_y = points[i].y;
      }

      if (points[i].x > max_x) {
        max_x = points[i].x;
      }

      if (points[i].y > max_y) {
        max_y = points[i].y;
      }
    }
  }
  max_point.x = max_x;
  max_point.y = max_y;
  min_point.x = min_x;
  min_point.y = min_y;
}

void normalize_points (const int size) {

  double sclX = 0, sclY = 0;
  int i = 0;

  // x = (xi - xmin) * (1 / (xmax - xmin))
  sclX = (double)((max_point.x == min_point.x) ?
    0.0 : 1.0 / (max_point.x - min_point.x));

  sclY = (double)((max_point.y == min_point.y) ?
    0.0 : 1.0 / (max_point.y - min_point.y));

  #pragma omp parallel shared(size, points, min_point, max_point, sclX, sclY, norm_points) private (i)
  {
    #pragma omp for schedule(static, size/ n_threads)
    for (i = 0; i < size; ++i) {
      norm_points[i].x = sclX * (points[i].x - min_point.x);
      norm_points[i].y = sclY * (points[i].y - min_point.y);
    }
  }
}

void norm (const int size) {

  find_min_max_points(size);
  normalize_points(size);

} 

void set_points_values(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    points[i].x = (double)(rand());
    points[i].y = (double)(rand());
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

    points = (struct point*) malloc(sizeof(struct point) * size);
    norm_points = (struct point*) malloc(sizeof(struct point) * size);

    set_points_values(size);
    set_threads_number();
    norm(size);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
        printf("%f ", norm_points[i].x);
        printf("%f\n", norm_points[i].y);
      }
    }

    free(points);
    free(norm_points);

  } else {


  }

  return 0;
}