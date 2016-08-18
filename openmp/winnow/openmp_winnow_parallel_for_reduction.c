#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <limits.h>
#include "omp.h"

struct point {
  double x;
  double y;
};

struct weighted_point {
  int weight;
  struct point position;
};

void fill_vector (int size) {
  
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (mask[i * size + j]) {
        struct weighted_point value;
        value.weight = matrix[i* size + j];
        value.position.x = i;
        value.position.y = j;
        vector[size--] = value;
      }
    }
  }
}

int partition(struct weighted_point* vector, int l, int r) {
   int pivot, i, j, t;
   double x = 0;
   double y = 0;

   pivot = vector[l].weight;
   i = l;
   j = r + 1;
    
   for( ; ; ) {
   
    do ++i; while( vector[i].weight <= pivot && i <= r );
    do --j; while( vector[j].weight > pivot );
    if( i >= j ) break;
    
    t = vector[i].weight; vector[i].weight = vector[j].weight; vector[j].weight = t;
    x = vector[i].position.x; vector[i].position.x = vector[j].position.x; vector[j].position.x = x;
    y = vector[i].position.y; vector[i].position.y = vector[j].position.y; vector[j].position.y = y;
   
   }
   
   t = vector[l].weight; vector[l].weight = vector[j].weight; vector[j].weight = t;
   x = vector[l].position.x; vector[l].position.x = vector[j].position.x; vector[j].position.x = x;
   y = vector[l].position.y; vector[l].position.y = vector[j].position.y; vector[j].position.y = y;

   return j;
   
}

void quick_sort(struct weighted_point* vector, int l, int r)
{
   int i = 0;

   if( l < r ) 
   {
       i = partition( vector, l, r);
       quick_sort( vector, l, i - 1);
       quick_sort( vector, i + 1, r);
   }
  
}

void fill_point_vector(struct weighted_point* vector, int vector_size, struct point* point_vector, int nelts) {

  int i = 0;
  int chunk = vector_size / nelts;
  for (i = 0; i < nelts; ++i) {
    int vector_index = i * chunk;
    point_vector[i] = vector[vector_index].position;
  }
}

void winnow (int size) {

  int i, j;
  struct weighted_point* vector;
  struct point* point_vector;

  vector = (struct weighted_point*)malloc(sizeof(struct weighted_point) * vector_size);
  point_vector = (struct point*)malloc(sizeof(struct point) * nelts);

  fill_vector(matrix, mask, size, vector, vector_size);
  quick_sort(vector, 0, vector_size);
  fill_point_vector(vector, vector_size, point_vector, nelts);

  return point_vector;
}

static double* matrix;
static double* vector;
static struct point* points;

void set_values_matrix(int nrows, int ncols) {
  int i, j;
  for (i =  0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      matrix[i*ncols +j] = rand();
    }
  }
}

void set_values_mask(int nrows, int ncols) {
  int i, j;
  for (i =  0; i < nrows; i++) {
    for (j = 0; j < ncols; j++) {
      mask[i*ncols +j] = rand() % 2;
    }
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

    matrix = (double*) malloc (sizeof (double) * size * size);
    vector = (double*) malloc (sizeof (double) * size);
    points = (struct point*) malloc(sizeof (struct point) * size);

    set_points_values(size);
    set_threads_number(num_threads);
    outer(size);

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          printf("%e ", matrix[i* size + j]);
        }
        printf("\n");
      }

      printf("\n");

      for (i = 0; i < size; i++) {
        printf("%e ", vector[i]);
      }

      printf("\n");

    }
  } else {


  }

  return 0;
}