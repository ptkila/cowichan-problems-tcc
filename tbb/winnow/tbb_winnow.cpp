#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_scan.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_reduce.h"

static std::pair<int, int> *points;
static std::pair<int, std::pair<int, int> > *values;
static int* matrix;
static int* mask;
static int* count_per_line;
static int* total_count;

typedef tbb::blocked_range<size_t> range;

class ScanSum {
  int sum;

public:
  ScanSum(): sum(0) {}
  template<typename Tag>
  void operator()(range r, Tag) {
    int res = sum;
    auto begin = r.begin();
    auto end = r.end();
    for (size_t i = begin; i != end; ++i) {
      res += count_per_line[i];
      if (Tag::is_final_scan()) {
        total_count[i] = res;
      }
    }
    sum = res;
  }
  ScanSum(ScanSum& other, tbb::split) : sum(0) {}
  void reverse_join(ScanSum& other) { sum += other.sum; }
  void assign(ScanSum& other) { sum = other.sum; }
};

void winnow(int nrows, int ncols, int nelts) {
  int count = 0;
  count = parallel_reduce(
    range(0, nrows), 0,
    [=](range r, int result)->int {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; i++) {
        int cur = 0;
        for (int j = 0; j < ncols; j++) {
          cur += mask[i*ncols + j];
        }
        result += count_per_line[i + 1] = cur;
      }
      return result;
    },
    [](int x, int y)->int {
      return x + y;
    });

  ScanSum scan_sum;
  tbb::parallel_scan(
    range(0, nrows + 1),
    scan_sum,
    tbb::auto_partitioner());

  tbb::parallel_for(
    range(0, nrows),
    [=](range r) {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; i++) {
        int count = total_count[i];
        for (int j = 0; j < ncols; j++) {
          if (mask[i*ncols + j]) {
            int v = matrix [i * ncols + j];
            values[count] = (std::make_pair(v, std::make_pair(i, j)));
            count++;
          }
        }
      }
    });

  sort(values, values + count);

  size_t n = count;
  size_t chunk = n / nelts;

  for (int i = 0; i < nelts; i++) {
    int index = i * chunk;
    points[i] = values[index].second;
  }
}

void setValuesMatrix(int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i*size + j] = std::rand();
    }
  }
}

void setValuesMask(int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      mask[i*size + j] = std::rand()%2;
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

    matrix = (int *) malloc (sizeof (int) * size * size);
    mask = (int *) malloc (sizeof (int) * size * size);
    setValuesMatrix(size);
    setValuesMask(size);

    total_count = (int*) calloc (sizeof(int), (size + 1));
    count_per_line = (int*) calloc (sizeof(int), (size + 1));
    points = (std::pair <int, int> *) malloc (sizeof (std::pair <int, int>) * size);
    values = (std::pair <int, std::pair <int, int> > *) malloc (sizeof (std::pair <int, std::pair <int, int> >) * size);

    setThreadsNumber(num_threads);
    winnow(size, size, size);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        printf("%d %d\n", points[i].first, points[i].second);
      }
      printf("\n");
    }

  } else {

  }
  return 0;
}