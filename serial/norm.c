#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>

struct point {
  double x;
  double y;
};

static struct point* norm_points;
static struct point* points;
static struct point min_point;
static struct point max_point;

void find_min_max_points (const int size) {

  min_point.x = DBL_MAX;
  min_point.y = DBL_MAX;
  max_point.x = -DBL_MAX;
  max_point.y = -DBL_MAX;
  int i;
  
  for (i = 0; i < size; ++i) {

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

void normalize_points (const int size) {

  double sclX = 0, sclY = 0;
  int i;

  // x = (xi - xmin) * (1 / (xmax - xmin))
  sclX = (double)((max_point.x == min_point.x) ?
    0.0 : 1.0 / (max_point.x - min_point.x));

  sclY = (double)((max_point.y == min_point.y) ?
    0.0 : 1.0 / (max_point.y - min_point.y));

  for (i = 0; i < size; ++i) {
    norm_points[i].x = sclX * (points[i].x - min_point.x);
    norm_points[i].y = sclY * (points[i].y - min_point.y);
  }
}

void norm (const int size) {

  find_min_max_points(size);
  normalize_points(size);

} 

void set_points_values(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    points[i].x = (double)rand();
    points[i].y = (double)rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);

    points = (struct point*) malloc (sizeof(struct point) * size);
    norm_points = (struct point*) malloc (sizeof(struct point) * size);

    set_points_values(size);
    norm(size);

    if (print == 1) {
      int i;
      for (i = 0; i < size; ++i) {
        printf("%f ", norm_points[i].x);
        printf("%f\n", norm_points[i].y);
      }
    }

    free(points);
    free(norm_points);

  } else {

    printf("programa <tamanho> <printar>\n");

  }

  return 0;
}