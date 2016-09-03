#include "tbb/tbb.h"
#include "tbb/blocked_range2d.h"
#include <iostream>

typedef tbb::blocked_range2d<size_t, size_t> range2d;
static double* matrix;
static double* vector;
static double* result;
static int numThreads;

void product(const int size) {
  tbb::parallel_for(range2d(0, size, 0, size),[&](const range2d& r) {
    size_t r_end = r.rows().end();
    for (size_t i = r.rows().begin(); i != r_end; i++) {
      double sum = 0;
      size_t c_end = r.cols().end();
      for (size_t j = r.cols().begin(); j != c_end; j++) {
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
      matrix[i*size + j] = (double) rand();
    }
  }
}

void setVectorValues(const int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = (double)rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

   srand (time(NULL));
   int size = atoi(argv[1]);
   numThreads = atoi(argv[2]);
   int print = atoi(argv[3]);

   matrix = new double[size * size];
   result = new double[size];
   vector = new double[size];

   setThreadsNumber();
   setMatrixValues(size);
   setVectorValues(size);
   
   product(size);

   if (print == 1) {
    for (int i = 0; i < size; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
  }

  delete[] matrix;
  delete[] result;
  delete[] vector;

} else {

} 

return 0;

}