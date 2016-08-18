#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/parallel_scan.h"
#include "tbb/task_scheduler_init.h"

static int *matrix;
static int *mask;
static int *histogram;

typedef tbb::blocked_range<size_t> range;

void thresh(int nrows, int ncols, int percent) {
  int nmax = 0;
  nmax = tbb::parallel_reduce(
    range(0, nrows), 0,
    [=](range r, int result)->int {
      for (size_t i = r.begin(); i != r.end(); i++) {
        for (int j = 0; j < ncols; j++) {
          result = std::max(result , (int)matrix[i*ncols + j]);
        }
      }
      return result;
    },
    [](int x, int y)->int {
      return std::max(x, y);
    });

  tbb::parallel_for(
    range(0, nrows),
    [=](range r) {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; i++) {
        for (int j = 0; j < ncols; j++) {
          histogram[matrix[i*ncols + j]]++;
        }
      }
    });

  int count = (nrows * ncols * percent) / 100;

  int prefixsum = 0;
  int threshold = nmax;

  for (int i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[i];
    threshold = i;
  }

  tbb::parallel_for(
    range(0, nrows),
    [=](range r) {
      for (size_t i = r.begin(); i != r.end(); ++i) {
        for (int j = 0; j < ncols; j++) {
          mask[i*ncols + j] = matrix[i*ncols + j] >= threshold;
        }
      }
    });
}

void setValuesMatrix (int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
     matrix[i*size + j] = std::rand() % 255;
   }
 }
}

void setThreadsNumber(int threadsNumber) {

  tbb::task_scheduler_init init(threadsNumber);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = (int*) malloc (sizeof (int) * size * size);
    mask = (int*) malloc (sizeof (int) * size * size);
    histogram = (int*) calloc (sizeof (int), 255);

    setValuesMatrix(size);
    setThreadsNumber(num_threads);
    thresh(size, size, percent);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%hhu ", mask[i*size + j]);
        }
        printf("\n");
      }
    }
  }
  return 0;
}