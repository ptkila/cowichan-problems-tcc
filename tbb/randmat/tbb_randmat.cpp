#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_scheduler_init.h"

typedef tbb::blocked_range<size_t> range;
const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;

void randmat(const int size, const int seed) {
  tbb::parallel_for(range(0, size),[=](range r) {
      auto begin = r.begin();
      auto end = r.end ();
      int s = 0; 
      for (size_t i = begin; i != end; ++i) {
        for (int j = 0; j < size; j++) {
          s = VAL_A * (seed + i + j) + VAL_B;
          matrix[i*ncols + j] = seed % 100;
        }
      }
    });
}

void setThreadsNumber(const int threadsNumber) {

  tbb::task_scheduler_init init(threadsNumber);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size * size];
    setThreadsNumber(numThreads);
    randmat(size, rand());

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%d ", matrix[i*size + j]);
        }
        printf("\n");
      }
    }

  } else {



  }

  return 0;

}