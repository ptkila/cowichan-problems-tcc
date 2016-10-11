#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <cilk/reducer_opadd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct point_w {

	int weight, i, j;

};

static int* matrix;
static int* mask;
static int* count_per_line;
static struct point_w* ev_values;
static struct point_w* points;

int compare (const void * a, const void * b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void fill_values (const int size) {

  int i, j;

  cilk_for (i = 0; i < size; ++i) {
    int count = count_per_line[i];
    for (j = 0; j < size; ++j) {
      if (mask[i*size + j] == 1) {
        ev_values[count].weight = matrix[i*size + j];
        ev_values[count].i = i;
        ev_values[count].j = j;
        ++count;
      }
    }
  }
}

int count_points_reduce (const int size) {

  int i, j;

  CILK_C_REDUCER_OPADD(total_sum, int, 0);
  CILK_C_REGISTER_REDUCER(total_sum);
  cilk_for (i = 0; i < size; ++i) {
    int tmp_sum = 0;
    for (j = 0; j < size; ++j) {
      tmp_sum += mask[i*size + j];
    } 
    count_per_line[i + 1] = tmp_sum;
    REDUCER_VIEW(total_sum) += tmp_sum;
  }
  int len = REDUCER_VIEW(total_sum);
  CILK_C_UNREGISTER_REDUCER(total_sum);
  
  return len; 

}

void prefix_sum_for (const int size) {

  if (size == 1) { 

    return;
    
  } else {

    int itr, i;
    for (itr = 1; (1 << itr) <= size; ++itr) {
      int tmp_iter = 1 << itr;
      cilk_for (i = tmp_iter; i <= size; i += tmp_iter) {
        count_per_line[i] += count_per_line[i - (1 << (itr - 1))];
      }
    }
    itr--;
    while (itr >= 0) {
      int tmp_iter = 1 << itr;
      cilk_for (i = tmp_iter; i <= size; i += tmp_iter) {
        if ((i >> itr) == 1){ 
          count_per_line[i] = count_per_line[i];
        } else if ((i >> itr) % 2 == 1) { 
          count_per_line[i] += count_per_line[i - tmp_iter];
        }
      }
      itr--;
    }
  }
}

void fill_points(const int len, const int nelts) {
  const int chunk = len/ nelts;
  int i;
  cilk_for (i = 0; i < nelts; ++i) {
    points[i] = ev_values[i*chunk];
  }
}

void winnow (const int size, const int nelts) {

  int len = count_points_reduce(size);
  ev_values = (struct point_w*) malloc (sizeof(struct point_w) * len);

  prefix_sum_for(size);

  fill_values(size);

  qsort(ev_values, len, sizeof(*ev_values), compare);

  points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);
  fill_points(len, nelts);

}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size +j] = rand();
    }
  }
}

void set_values_mask(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      mask[i*size +j] = rand() % 2;
    }
  }
}

void set_threads_number (const int n_threads) {

  char threads[2];
  sprintf(threads,"%d", n_threads);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

}

int main(int argc, char *argv[]) {
  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    count_per_line = (int*) malloc (sizeof(int) * (size + 1)); // i = 1
    int nelts = 5;

    set_threads_number(n_threads);
    set_values_matrix(size);
    set_values_mask(size);

    winnow(size, nelts);

    if (print) {
      int i;
      for (i = 0; i < nelts; ++i) {
        printf("%d %d %d\n", points[i].i, 
          points[i].j, points[i].weight);
      }
    }

    free (matrix);
    free (mask);
    free (count_per_line);
    free (ev_values);
    free (points);

  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;
}