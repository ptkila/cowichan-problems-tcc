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

typedef tbb::blocked_range<size_t> range;
static int* matrix;
static int* mask;
static int* histogram;
static int numThreads;

int findMax (const int size) {
  tbb::parallel_reduce(range(0, size), 0, [=](range r, int result)->int {
      for (size_t i = r.begin(); i != r.end(); i++) {
        for (int j = 0; j < size; j++) {
          result = std::max(result, matrix[i*size + j]);
        }
      }
      return result;
    },
    [](int x, int y)->int {
      return std::max(x, y);
    });
}

void fillHistogram (const int size) {

  tbb::parallel_for(range(0, size),[=](range r) {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; i++) {
        for (int j = 0; j < size; j++) {
          histogram[matrix[i*size + j]]++;
        }
      }
    });
}

void fillMask (const int size, const int threshold) {
  tbb::parallel_for(range(0, size),[=](range r) {
      for (size_t i = r.begin(); i != r.end(); ++i) {
        for (int j = 0; j < size; j++) {
          mask[i*size + j] = matrix[i*size + j] >= threshold;
        }
      }
    });
}

void thresh(int size, int percent) {
  
  int nmax = findMax(size);
  int count = (size * size * percent) / 100;

  fillHistogram(size);

  int prefixsum = 0;
  int threshold = nmax;

  for (int i = nmax; i >= 0 && prefixsum <= count; i--) {
    prefixsum += histogram[i];
    threshold = i;
  }

  fillMask(size, threshold);
}

void setValuesMatrix (int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
     matrix[i*size + j] = std::rand() % 255;
   }
 }
}

void setThreadsNumber() {

  tbb::task_scheduler_init init(numThreads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = int[size * size];
    mask = int[size * size];
    histogram = int[256];

    setThreadsNumber();
    setValuesMatrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%hhu ", mask[i*size + j]);
        }
        printf("\n");
      }
    }

    delete[] matrix;
    delete[] histogram;
    delete[] mask;

  } else {


  }
  return 0;
}