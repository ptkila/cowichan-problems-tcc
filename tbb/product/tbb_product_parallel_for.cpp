#include "tbb/tbb.h"
#include <iostream>

typedef tbb::blocked_range<size_t> range;
static double* matrix;
static double* vector;
static double* result;
static int numThreads;

void product(const int size) {
  tbb::parallel_for(range(0, size),[&](const range& r) -> void {
    size_t r_end = r.end();
    for (size_t i = r.begin(); i != r_end; ++i) {
      double sum = 0.0;
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
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 10;
    }
  }
}

void setVectorValues(const int size) {
  for (int i = 0; i < size; ++i) {
    vector[i] = rand() % 10;
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
    for (int i = 0; i < size; ++i) {
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