#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <pthread.h>
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

static struct point_w* ev_values;
static struct point_w* points;

static int nelts;
static int n_threads;

int compare (const void * a, const void * b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void fill_values(const int begin, const int end, const int size) {
  
  if (begin + 1 == end) {
    int i;
    int count = count_per_line[begin];
    for (i = 0; i < size; ++i) {
      if (mask[begin*size + i] == 1) {
        ev_values[count].weight = matrix[begin*size + i];
        ev_values[count].i = begin;
        ev_values[count].j = i;
        count++;
      }
    }
  
    return;
  
  } else {

    int middle = begin + (end - begin) / 2;
    cilk_spawn fill_values(begin, middle, size);
    cilk_spawn fill_values(middle, end, size);
  
  }
}

int count_points(const int begin, const int end, const int size) {
  
  if (begin + 1 == end) {
    
    int i, count = 0;
    for (i = 0; i < size; ++i) {
      count += mask[begin*size + i];
    }
    count_per_line[begin + 1] = count;
    return count;
  
  } else {

    int middle = begin + (end - begin)/ 2;
    int left = cilk_spawn count_points(begin, middle, size);
    int right = cilk_spawn count_points(middle, end, size);
    cilk_sync;
    return left + right;
  
  }
}

void scan_update_elements(const int begin, const int end, const int itr) {
  
  if (end - begin <= itr) {
  
    return;
  
  } else if (end - begin <= 2 * itr) {

    count_per_line[begin + itr] = count_per_line[begin] + count_per_line[begin + itr];
  
  } else {

    int count = (end - begin) / itr;
    count /= 2;
    count += count % 2;
    
    int middle = begin + count * itr;
    cilk_spawn scan_update_elements(begin, middle, itr);
    cilk_spawn scan_update_elements(middle, end, itr);
  
  }
}

void scan(const int begin, const int end, const int iter) {
  
  if (end - begin > iter) {

    cilk_spawn scan_update_elements(begin, end, iter);
    cilk_sync;
    
    cilk_spawn scan(begin + iter, end, 2 * iter);
    cilk_sync;
    
    cilk_spawn scan_update_elements(begin + iter, end, iter);
    cilk_sync;
  
  } else {

    return;
  
  }
}

void prefix_sum_rec(const int size) {

  cilk_spawn scan(0, size, 1);
  cilk_sync;

}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
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

void fill_points(const int len) {
  int i;
  int chunk = len/ nelts;
  cilk_for(i = 0; i < nelts; ++i) {
    points[i] = ev_values[i*chunk];
  }
}

void winnow (const int size) {

  int len = cilk_spawn count_points(0, size, size);
  cilk_sync;

  ev_values = (struct point_w*) malloc (sizeof(struct point_w) * len);

  cilk_spawn prefix_sum_rec(size);
  cilk_sync;

  cilk_spawn fill_values(0, size, size);
  cilk_sync;

  qsort(ev_values, len, sizeof(*ev_values), compare);
  
  // Garante nelts <= n_points && > 0
  nelts = rand() % len;
  if (!nelts) {
    nelts = 1;
  }
  
  points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);
  fill_points(len);

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
    count_per_line = (int*) malloc (sizeof(int) * (size + 1)); // i = 1

    set_threads_number();
    set_values_matrix(size);
    set_values_mask(size);

    winnow(size);

    if (print) {
      int i;
      for (i = 0; i < nelts; ++i) {
        printf("%d %d %d\n", points[i].i, points[i].j, points[i].weight);
      }
    }

    free (matrix);
    free (mask);
    free (ev_values);
    free (points);

  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;
}