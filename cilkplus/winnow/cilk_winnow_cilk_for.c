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

struct point {

	int i, j;

};

static int* matrix;
static int* mask;
static struct point_w* points;
static int nelts;
static int n_threads;
static int counter;
static int total_sum;

int compare (const void * a, const void * b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void fill_vector(const int size) {
  int i, j;
  cilk_for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (mask[i*size + j] == 1) {
        points[counter].weight = matrix[i*size + j];
        points[counter].i = i;
        points[counter].j = j;
        counter++;
      }
    }
  }
}

void fill_ev_points() {

	//int chunk =  / nelts;

}

void winnow (const int size) {

  fill_vector(size);
  qsort (points, nelts, sizeof(struct point_w), compare);
  
  // Seta número de pontos
  //nelts = rand()%counter;
  //fill_ev_points(size);

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
      if (mask[i*size +j] == 1){ 
        nelts++;
      }
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

    set_threads_number();
    set_values_matrix(size);
    set_values_mask(size);

    points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);

    winnow(size);

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