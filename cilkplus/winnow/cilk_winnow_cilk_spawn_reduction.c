#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct sPoint {
  int value, i, j;
} Point;
static int *matrix;
static int *mask;
static int *count_per_line;
static Point *points;
static Point *values;

int compare(const void* vl, const void* vr) {
  const Point* l = (Point*) vl, *r = (Point*)vr;
  return (l->value - r->value);
}

int reduce_sum(int begin, int end, int ncols) {
  int middle = begin + (end - begin) / 2;
  int left, right, res, i;
  if (begin + 1 == end) {
    res = mask[begin*ncols +0];
    for (i = 1; i < ncols; i++) {
      res += mask[begin*ncols +i];
    }
    return count_per_line[begin + 1] = res;
  }
  left = cilk_spawn reduce_sum(begin, middle, ncols);
  right = cilk_spawn reduce_sum(middle, end, ncols);
  cilk_sync;
  return left + right;
}

void scan_update_elements(int begin, int end, int* array, int size) {
  int middle, count;
  if (end - begin <= size) {
    return;
  } else if (end - begin <= 2 * size) {
    array[begin + size] = array[begin] + array[begin + size];
  } else {
    count = (end - begin) / size;
    count /= 2;
    count += count % 2;
    middle = begin + count * size;
    cilk_spawn scan_update_elements(begin, middle, array, size);
    cilk_spawn scan_update_elements(middle, end, array, size);
  }
}

void scan_impl(int begin, int end, int* array, int size) {
  if (end - begin > size) {
    cilk_spawn scan_update_elements(begin, end, array, size);
    cilk_sync;
    cilk_spawn scan_impl(begin + size, end, array, 2 * size);
    cilk_sync;
    cilk_spawn scan_update_elements(begin + size, end, array, size);
    cilk_sync;
  }
}

void scan(int n, int* array) {
  cilk_spawn scan_impl(0, n, array, 1);
}

void prefix_sum(int n) {
  cilk_spawn scan(n, count_per_line);
}

void fill_values(int begin, int end, int ncols) {
  int middle = begin + (end - begin) / 2;
  int count, j;
  if (begin + 1 == end) {
    count = count_per_line[begin];
    for (j = 0; j < ncols; j++) {
      if (mask[begin*ncols +j] == 1) {
        values[count].value = matrix[begin*ncols +j];
        values[count].i = begin;
        values[count].j = j;
        count++;
      }
    }
    return;
  }
  cilk_spawn fill_values(begin, middle, ncols);
  cilk_spawn fill_values(middle, end, ncols);
}

void winnow(int nrows, int ncols, int nelts) {
  int i, n =  0, chunk, index;

  n = cilk_spawn reduce_sum(0, nrows, ncols);
  cilk_sync;

  cilk_spawn prefix_sum(nrows + 1);
  cilk_sync;

  cilk_spawn fill_values(0, nrows, ncols);
  cilk_sync;

  qsort(values, n, sizeof(*values), compare);

  chunk = n / nelts;

  for (i = 0; i < nelts; i++) {
    index = i * chunk;
    points[i] = values[index];
  }
}

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

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char *argv[]) {
  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int i, j;

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    values = (Point*) malloc (sizeof(Point) * size * size);

    set_values_matrix(size, size);
    set_values_mask(size, size);

    count_per_line = (int*) malloc (sizeof(int) * (size + 1));
    points = (Point*) malloc (sizeof(Point) * size);

    set_threads_number(num_threads);
    cilk_spawn winnow(size, size, size);
    cilk_sync;

    if (print == 1) {
      for (i = 0; i < size; i++) {
        printf("%d %d\n", points[i].i, points[i].j);
      }
      printf("\n");
    }
  } else {

    printf("programa <tamanho> <num de num_threads> <printar>\n");

  }

  return 0;
}