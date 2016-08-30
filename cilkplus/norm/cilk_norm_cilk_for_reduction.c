#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <cilk/reducer_max.h>
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

void find_min_max_points (const int size) {

  int i;

  CILK_C_REDUCER_MAX(maxX, double, -DBL_MAX);
  CILK_C_REDUCER_MAX(maxY, double, -DBL_MAX);
  CILK_C_REDUCER_MIN(minX, double, DBL_MAX);
  CILK_C_REDUCER_MIN(minY, double, DBL_MAX);
  
  CILK_C_REGISTER_REDUCER(maxX);
  CILK_C_REGISTER_REDUCER(maxY);
  CILK_C_REGISTER_REDUCER(minX);
  CILK_C_REGISTER_REDUCER(minY);

  cilk_for(i = 0; i < size; ++i) {
    CILK_C_REDUCER_MAX_CALC(maxX, points[i].x);
    CILK_C_REDUCER_MAX_CALC(maxY, points[i].y);
    CILK_C_REDUCER_MIN_CALC(minX, points[i].x);
    CILK_C_REDUCER_MIN_CALC(minY, points[i].y); 
  }

  CILK_C_UNREGISTER_REDUCER(maxX);
  CILK_C_UNREGISTER_REDUCER(maxY);
  CILK_C_UNREGISTER_REDUCER(minX);
  CILK_C_UNREGISTER_REDUCER(minY);

  max_point.x = REDUCER_VIEW(maxX);
  max_point.y = REDUCER_VIEW(maxY);
  min_point.x = REDUCER_VIEW(minX);
  min_point.y = REDUCER_VIEW(minY);

  /*
  printf("%f \n", max_point.x);
  printf("%f \n", max_point.y);
  printf("%f \n", min_point.x);
  printf("%f \n", min_point.y);
  */
}

void normalize_points (const int size) {

  double sclX = 0, sclY = 0;
  int i;

  // x = (xi - xmin) * (1 / (xmax - xmin))
  sclX = (double)((max_point.x == min_point.x) ?
    0.0 : 1.0 / (max_point.x - min_point.x));

  sclY = (double)((max_point.y == min_point.y) ?
    0.0 : 1.0 / (max_point.y - min_point.y));

  cilk_for (i = 0; i < size; i++) {
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
  /*
  for (i = 0; i < size; ++i) {
    printf("%f\n", points[i].x);
    printf("%f\n", points[i].y);
    printf("\n");
  }
  */
}

void set_threads_number(const int t_num) {

 char threads[2];
 sprintf(threads,"%d", t_num);
 __cilkrts_end_cilk();  
 __cilkrts_set_param("nworkers", threads);

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