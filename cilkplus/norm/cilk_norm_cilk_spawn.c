#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct point {
  double x;
  double y;
};

static struct point* norm_points;
static struct point* points;
static struct point min_point;
static struct point max_point;
static int n_threads;

void find_max_min_points (const int begin, const int end, const int size) {

  if(begin + 1 == end) {

    if (points[begin].x < min_point.x) {
      min_point.x = points[begin].x;
    }

    if (points[begin].y < min_point.y) {
      min_point.y = points[begin].y;
    }

    if (points[begin].x > max_point.x) {
      max_point.x = points[begin].x;
    }

    if (points[begin].y > max_point.y) {
      max_point.y = points[begin].y;
    }

    return;

  } else {

    int middle = begin + (end - begin)/ 2;
    cilk_spawn find_max_min_points(begin, middle, size);
    cilk_spawn find_max_min_points(middle, end, size);

  }
}

void normalize_points (const int begin, const int end, const double sclX, const double sclY, 
  const int size) {

  if(begin + 1 == end){

      norm_points[begin].x = sclX * (points[begin].x - min_point.x);
      norm_points[begin].y = sclY * (points[begin].y - min_point.y);

    return;

  } else {

    int middle = begin + (end - begin)/ 2;
    cilk_spawn normalize_points(begin, middle, sclX, sclY, size);
    cilk_spawn normalize_points(middle, end, sclX, sclY, size);

  }
}

void norm (const int size) {

  cilk_spawn find_max_min_points(0, size, size);
  cilk_sync;

  double sclX = (double)((max_point.x == min_point.x) ?
    0.0 : 1.0 / (max_point.x - min_point.x));
  double sclY = (double)((max_point.y == min_point.y) ?
    0.0 : 1.0 / (max_point.y - min_point.y));

  cilk_spawn normalize_points(0, size, sclX, sclY, size);
  cilk_sync;
} 

void set_points_values(const int size) {
  int i;
  
  for (i = 0; i < size; ++i) {
    points[i].x = (double)rand();
    points[i].y = (double)rand();
  }
}

void set_threads_number() {

 char threads[2];
 sprintf(threads,"%d", n_threads);
 __cilkrts_end_cilk();  
 __cilkrts_set_param("nworkers", threads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    points = (struct point*) malloc(sizeof(struct point) * size);
    norm_points = (struct point*) malloc(sizeof(struct point) * size);
    min_point.x = DBL_MAX;
    min_point.y = DBL_MAX;
    max_point.x = -DBL_MAX;
    max_point.y = -DBL_MAX;

    set_threads_number();
    set_points_values(size);
    
    cilk_spawn norm(size);
    cilk_sync;

    if (print == 1) {
      int i;
      for (i = 0; i < size; i++) {
        printf("%g ", norm_points[i].x);
        printf("%g\n", norm_points[i].y);
      }
    }

    free(points);
    free(norm_points);

  } else {


  }

  return 0;
}