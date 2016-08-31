#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <cilk/reducer_opadd.h>
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

int reduce_sum (int nrows, int ncols) {

  int i, j, sum;
  CILK_C_REDUCER_OPADD(r, double, 0.0);
  CILK_C_REGISTER_REDUCER(r);

  cilk_for (i = 0; i < nrows; i++) {
    int tmp_sum = 0;
    for (j = 0; j < ncols; j++) {
      tmp_sum += mask[i * ncols + j];
    }
    count_per_line[i + 1] = tmp_sum;
    REDUCER_VIEW(r) += tmp_sum;
  }
  CILK_C_UNREGISTER_REDUCER(r);
  
  sum = REDUCER_VIEW(r);
  return sum;
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
    scan_update_elements(middle, end, array, size);
  }
}

void scan_impl(int begin, int end, int* array, int size) {
  if (end - begin > size) {
    scan_update_elements(begin, end, array, size);
    scan_impl(begin + size, end, array, 2 * size);
    scan_update_elements(begin + size, end, array, size);
  }
}

void scan(int n, int* array) {
  scan_impl(0, n, array, 1);
}

void prefix_sum(int n) {
  scan(n, count_per_line);
}

void fill_values(int nrows, int ncols) {
  int count, j, i;
  cilk_for (i = 0; i < nrows; ++i) {
    count = count_per_line[i];
    for (j = 0; j < ncols; j++) {
      if (mask[i*ncols +j] == 1) {
        values[count].value = matrix[i*ncols +j];
        values[count].i = i;
        values[count].j = j;
        count++;
      }
    }
  }
}

void winnow(int nrows, int ncols, int nelts) {
  int i, n, chunk, index;

  n = reduce_sum(nrows, ncols);

  prefix_sum(nrows + 1);
  fill_values(nrows, ncols);

  qsort(values, n, sizeof(*values), compare);

  chunk = n / nelts;
  points = (Point*) malloc (sizeof(Point) * nelts);
  cilk_for (int i = 0; i < nelts; i++) {
    index = i * chunk;
    points[i] = values[index];
  }
}

void set_values_matrix(int size) {
  int i, j;
  for (i =  0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size +j] = rand();
    }
  }
}

void set_values_mask(int size) {
  int i, j;
  for (i =  0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i*size +j] = rand() % 2;
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
    int i;
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);
    values = (Point*) malloc (sizeof(Point) * size * size);
    set_values_matrix(size, size);
    set_values_mask(size, size);

    count_per_line = (int*) calloc (size + 1, sizeof(int));

    set_threads_number(num_threads);
    winnow(size, size, size);

    if (print == 1) {
      for (i = 0; i < size; i++) {
        printf("%d %d %d\n", points[i].i, points[i].j, points[i].value);
      }
      printf("\n");
    }

  } else {



  }
  return 0;
}