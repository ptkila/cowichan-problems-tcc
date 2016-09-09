#include "tbb/tbb.h"
#include "tbb/blocked_range2d.h"
#include <iostream>

typedef tbb::blocked_range2d<size_t, size_t> range2d;
const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;
static int numThreads;

//[capture clause] (parameters) -> return-type {body}

void randmat(const int size, const int seed) {
  int s = 0;
  tbb::parallel_for(range2d(0, size, 0, size), [&](const range2d& r) {
    size_t r_end = r.rows().end();
    for (size_t i = r.rows().begin(); i != r_end; ++i) {
        size_t c_end = r.cols().end();
        for (size_t j = r.cols().begin(); j != c_end; ++j) {
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
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
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