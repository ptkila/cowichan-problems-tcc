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

double sqr(const double x) {

  return x * x;

}

double distance(const struct point a, const struct point b) {

  return sqrt(sqr(a.i - b.i) + sqr(a.j - b.j));

}

void outer(const int size) {
  int i, j;
  struct point origin;
  
  for (i = 0; i < size; ++i) {
    double nmax = 0;
    for (j = 0; j < size; ++j) {
      if (i != j) {
        matrix [i*size + j] = distance(points[i], points[j]);
        nmax = fmax(nmax, matrix [i*size + j]);
      }      
    }
    matrix [i*size + i] = nmax * size;
    vector [i] = distance(origin, points[i]);
  }
}


void set_vector_of_points(const int size) {
  int i;
  for (i = 0; i < size; ++i) {
    points[i].i = rand();
    points[i].j = rand();
  }
}

int main(int argc, char** argv) {
  
  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);

    matrix = (double*) malloc (sizeof(double) * size * size);
    vector = (double*) malloc (sizeof(double) * size);
    points = (struct point*) malloc (sizeof(struct point) * size);

    set_vector_of_points(size);
    
    outer(size);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
          printf("%g ", matrix[i*size + j]);
        }
        printf("\n");
      }
      printf("\n");

      for (i = 0; i < size; ++i) {
        printf("%g ", vector[i]);
      }
      printf("\n");
    }

    free(matrix);
    free(vector);
    free(points);

  } else {

    printf("programa <tamanho> <printar>\n");

  }

  return 0;
}