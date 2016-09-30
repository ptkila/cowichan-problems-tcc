#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <future>
#include "../ThreadPool.h"

static int* matrix;
static int* mask;
static int* histogram;

int compare(const int startIndex, const int endIndex, const int size) {
  int val = -1;
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      if (matrix[i * size + j] > val)
        val = matrix[i*size + j];
    }
  }
  return val;
}

void fillHistogram(const int startIndex, const int endIndex, const int size) {
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      ++histogram[matrix[i*size + j]];
    }
  }
}

void fillMask(const int startIndex, const int endIndex, const int size,
 const int threshold) {
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      mask[i*size + j] = (matrix[i*size + j] >= threshold);
    }
  }
}

int calcThreshold (const int percent, const int nMax, const int size) {
  int i;
  int count = (size * size * percent)/ 100;
  int prefixsum = 0;
  int threshold = nMax;

  for (i = nMax; i >= 0 && prefixsum <= count; --i) {
    prefixsum += histogram[i];
    threshold = i;
  }

  return threshold;
}

void thresh(const int size, const int percent, const int numThreads) {

  ThreadPool pool(numThreads);

  int nMax = pool.parallel_reducer(compare, size, "max");

  pool.parallel_for(fillHistogram, size);
  pool.waitAll();

  int threshold = calcThreshold(percent, nMax, size);

  pool.parallel_for(fillMask, size, threshold);
  pool.waitAll();

}

void setValuesMatrix(const int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 256;    
    }
  }
  /*
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      std::cout << matrix[i*size + j] << " ";    
    }
    std::cout << "\n";
  }
  */
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size * size];
    mask = new int[size * size];
    histogram = new int[256];
    int percent = 50;

    setValuesMatrix(size);
    thresh(size, percent, numThreads);

    if (print == 1) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          std:: cout << mask[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
    }

    delete[] matrix;
    delete[] mask;
    delete[] histogram;

  } else {


  }

  return 0;
}