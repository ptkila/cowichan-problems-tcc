#include <cstdio>
#include <cstring>
#include <iostream>
#include "tbb/tbb.h"

static double *matrix;
static double *vector;
static double *result;

typedef tbb::blocked_range<size_t> range;

void product(int nelts) {
  parallel_for(
    range(0, nelts),
    [&, nelts](range r) {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; ++i) {
        double sum = 0;
        for (int j = 0; j < nelts; ++j) {
          sum += matrix [i*nelts + j] * vector [j];
        }
        result [i] = sum;
      }
    });
}

void setThreadsNumber(int threadsNumber) {

  tbb::task_scheduler_init init(threadsNumber);

}

void setMatrixValues(int nelts) {
  for (int i = 0; i < nelts; i++) {
    for (int j = 0; j < nelts; j++) {
      matrix[i*nelts + j] = (float) rand();
    }
  }
}

void setVectorValues(int nelts) {
  for (int i = 0; i < nelts; i++) {
    vector[i] = (float)rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

   srand (time(NULL));
   int size = atoi(argv[1]);
   int num_threads = atoi(argv[2]);
   int print = atoi(argv[3]);

   matrix = (double *) malloc (sizeof(double) * size * size);
   vector = (double *) malloc (sizeof (double) * size);
   result = (double *) malloc (sizeof (double) * size);

   setMatrixValues(size);
   setVectorValues(size);
   setThreadsNumber(num_threads);
   product(size);

   if (print == 1) {
    for (int i = 0; i < size; i++) {
      printf("%g ", result[i]);
    }
    printf("\n");
  }
} else {



}

return 0;

}