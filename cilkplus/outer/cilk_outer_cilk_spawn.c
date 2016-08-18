#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct point {
  double i;
  double j;
};

static double *matrix;
static double *vector;
static struct point *points;

double sqr(const double x) {
  return x * x;
}

double distance(const struct point a, const struct point b) {
  return sqrt(sqr(a.i - b.i) + sqr(a.j - b.j));
}

void fill_matrix(const int begin, const int end, const int ncols) {
  int middle = begin + (end - begin) / 2;
  double nmax = -1;
  int j;
  struct point origin;
  
  if (begin + 1 == end) {
    for (j = 0; j < ncols; j++) {
      if (begin != j) {
        matrix[begin*ncols + j] = distance(points[begin], points[j]);
        nmax = fmax(nmax, matrix[begin*ncols + j]);
      }
      matrix[begin*ncols + begin] = nmax * ncols;
      vector[begin] = distance(origin, points[begin]);
    }
    return;
  }
  cilk_spawn fill_matrix(begin, middle, ncols);
  cilk_spawn fill_matrix(middle, end, ncols);
}

void outer(const int size) {
  cilk_spawn fill_matrix(0, size, size);
}

void set_vector_of_points(const int size) {
  int i;
  for (i =  0; i < size; i++) {
    points[i].i = rand();
    points[i].j = rand();
  }
}

void set_threads_number (const int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int i, j;

    matrix = (double*) malloc (sizeof(double) * size * size);
    vector = (double*) malloc (sizeof(double) * size);
    points = (struct point*) malloc (sizeof(Point) * size);

    set_vector_of_points(size);
    set_threads_number(num_threads);
    cilk_spawn outer(size);
    cilk_sync;

    if (print == 1) {
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%g ", matrix[i*size + j]);
        }
        printf("\n");
      }
      printf("\n");
      for (i = 0; i < size; i++) {
        printf("%g ", vector[i]);
      }
      printf("\n");
    }
  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");
    
  }

  return 0;
}