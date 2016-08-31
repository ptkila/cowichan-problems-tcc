#include <cstdio>
#include <cstring>
#include <iostream>
#include "tbb/tbb.h"

typedef tbb::blocked_range<size_t> range;
static double* matrix;
static double* vector;
static double* result;
static int numThreads;

void product(const int size) {
  parallel_for(range(0, size),[&, size](range r) {
    auto begin = r.begin();
    auto end = r.end();
    for (size_t i = begin; i != end; ++i) {
      double sum = 0;
      for (int j = 0; j < size; ++j) {
        sum += matrix [i*size + j] * vector [j];
      }
      result[i] = sum;
    }
  });
}

void setThreadsNumber() {

  tbb::task_scheduler_init init(numThreads);

}

void setMatrixValues(const int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i*size + j] = (float) rand();
    }
  }
}

void setVectorValues(const int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = (float)rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

   srand (time(NULL));
   int size = atoi(argv[1]);
   numThreads = atoi(argv[2]);
   int print = atoi(argv[3]);

   matrix = new int[size * size];
   result = new int[size * size];
   vector = new int[size];

   setThreadsNumber();
   setMatrixValues(size);
   setVectorValues(size);
   
   product(size);

   if (print == 1) {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        std::cout << result[i * size + j] << " ";
      }
      std::cout << std::endl;
    }
  }

  delete[] matrix;
  delete[] result;
  delete[] vector;

} else {

} 

return 0;

}