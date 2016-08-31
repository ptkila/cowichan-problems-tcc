#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_scheduler_init.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

typedef tbb::blocked_range<size_t> range;
const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;
static int numThreads;

void randmat(const int size, const int seed) {
  tbb::parallel_for(range(0, size),[=](range r) {
      auto begin = r.begin();
      auto end = r.end ();
      int s = 0; 
      for (size_t i = begin; i != end; ++i) {
        for (int j = 0; j < size; j++) {
          s = VAL_A * (seed + i + j) + VAL_B;
          matrix[i*size + j] = s % 100;
        }
      }
    });
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

    matrix = new int[size * size];
    
    setThreadsNumber();
    
    randmat(size, rand());

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          std::cout << matrix[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
    }

    delete[] matrix;

  } else {


  }

  return 0;

}