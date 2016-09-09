#include "tbb/tbb.h"
#include <iostream>

typedef tbb::blocked_range<size_t> range;
static int* matrix;
static int* mask;
static int* histogram;
static int numThreads;

int findMax (const int size) {
  int result = 0;
  tbb::parallel_for(range(0, size), [&](const range& r) {
    size_t r_end = r.end();
    for (size_t i = r.begin(); i != r_end; ++i) {
      for (int j = 0; j < size; ++j) {
        result = std::max(result, matrix[i*size + j]);
      }
    }
  });
  return result;
}

void fillHistogram (const int size) {
  tbb::parallel_for(range(0, size),[&](const range& r) {
    size_t r_end = r.end();
    for (size_t i = r.begin(); i != r_end; ++i) {
      for (int j = 0; j < size; ++j) {
        ++histogram[matrix[i*size + j]];
      }
    }
  });
}

void fillMask (const int size, const int threshold) {
  tbb::parallel_for(range(0, size),[&](const range& r) {
    size_t r_end = r.end();
    for (size_t i = r.begin(); i != r_end; ++i) {
      for (int j = 0; j < size; ++j) {
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
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
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

    matrix = new int[size * size];
    mask = new int[size * size];
    histogram = new int[256];

    setThreadsNumber();
    setValuesMatrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          std::cout << mask[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
    }

    delete[] matrix;
    delete[] histogram;
    delete[] mask;

  } else {


  }
  return 0;
}