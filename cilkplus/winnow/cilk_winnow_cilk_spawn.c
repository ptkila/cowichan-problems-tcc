#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct point_w {
  int weight, i, j;
};

static int* matrix;
static int* mask;
static int* count_per_line;
static struct point_w* points;
static int nelts;
static int counter;
static int n_threads;

int compare (const void* a, const void* b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void fill_vector(const int begin, const int end, const int size) {
  int middle = begin + (end - begin) / 2;
  if (begin + 1 == end) {
    int i;
    for (i = 0; i < size; i++) {
      if (mask[begin*size + i] == 1) {
        points[counter].weight = matrix[begin*size + i];
        points[counter].i = begin;
        points[counter].j = i;
        counter++;
      }
    }
    return;
  }
  cilk_spawn fill_vector(begin, middle, size);
  cilk_spawn fill_vector(middle, end, size);
  cilk_sync;
}

 void winnow (const int size) {

  cilk_spawn fill_vector(0, size, size);
  cilk_sync;
  qsort (points, nelts, sizeof(struct point_w), compare);

}

void set_values_matrix(int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size +j] = rand();
    }
  }
}

void set_values_mask(int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i*size +j] = rand() % 2;
    }
  }
}

void set_threads_number () {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

}

int main(int argc, char *argv[]) {
  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    counter = 0;
    nelts = 0;

    set_values_matrix(size);
    set_values_mask(size);

    points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);

    set_threads_number();

    cilk_spawn winnow(size);
    cilk_sync;

    printf("%d %d\n", nelts, counter);

    if (print == 1) {
      int i;
      for (i = 0; i < nelts; i++) {
        printf("%d %d %d\n", points[i].i, points[i].j, points[i].weight);
      }
      printf("\n");
    }

    free (matrix);
    free (mask);
    free (points);

  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;
}