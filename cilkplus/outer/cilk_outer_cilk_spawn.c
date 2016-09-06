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

static double* matrix;
static double* vector;
static struct point* points;
static struct point origin;
static int n_threads;

double sqr(const double x) {

  return x * x;

}

double distance(const struct point a, const struct point b) {

  return sqrt(sqr(a.i - b.i) + sqr(a.j - b.j));

}

void fill_matrix(const int begin, const int end, const int size) {
  if (begin + 1 == end) {
    int i;   
    double nmax = 0;
    for (i = 0; i < size; i++) {
      if (begin != i) {
        matrix[begin*size + i] = distance(points[begin], points[i]);
        nmax = fmax(nmax, matrix[begin*size + i]);
      }
      matrix[begin*size + begin] = nmax * size;
      vector[begin] = distance(origin, points[begin]);
    }
    
    return;
  
  } else {

    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_matrix(begin, middle, size);
    cilk_spawn fill_matrix(middle, end, size);
  
  }
}

void outer(const int size) {

  cilk_spawn fill_matrix(0, size, size);
  cilk_sync;
  
}

void set_vector_of_points(const int size) {
  int i;
  for (i = 0; i < size; i++) {
    points[i].i = rand();
    points[i].j = rand();
  }
}

void set_threads_number () {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (double*) malloc (sizeof(double) * size * size);
    vector = (double*) malloc (sizeof(double) * size);
    points = (struct point*) malloc (sizeof(struct point) * size);

    set_threads_number();
    set_vector_of_points(size);
    
    cilk_spawn outer(size);
    cilk_sync;

    if (print == 1) {
      int i, j;
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

    free(matrix);
    free(vector);
    free(points);

  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");
    
  }

  return 0;
}