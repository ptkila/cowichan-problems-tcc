#include "tbb.h"

struct Max {
  int max;
  int** matrix;
  int size;
  Max(int** matrix, int size) {
    this->matrix = matrix;
    this->size = size;
    this->max = 0;
  }
  Max( Max& m, tbb::split ) {this->max = 0;}
  
  void operator()( const tbb::blocked_range<int>& r ) {
    for (int i = r.begin(); i < r.end(); i++) {
      for (int j = 0; j < this->size; j++) {
        if (this->matrix[i][j] > this->max)
          this->max = this->matrix[i][j];
      }
    }
  }
  void join( Max& rhs ) {
    if (this->max > rhs.max)
      this->max = rhs.max;
  }
};

int reduceMax (int** matrix, int size) {

  Max max(matrix, size);
  tbb::parallel_reduce(
           tbb::blocked_range<int> (0, size),
           max
         );
  return max.max;
}

void fillHistogram(int** matrix, int size, int* histogram) {
  tbb::parallel_for(tbb::blocked_range2d<int, int> (0, size, 0, 0, size, 0), [ = ] (tbb::blocked_range2d<int, int> r)
  {
    for (int i = r.rows().begin(); i < r.rows().end(); i++) {
      for (int j = r.cols().begin(); j < r.cols().end(); j++) {
        histogram[matrix[i][j]]++;
      }
    }
  });
}

void fillMask(int** matrix, int size, int threshold, int** mask) {
  tbb::parallel_for(tbb::blocked_range2d<int, int> (0, size, 0, 0, size, 0), [ = ] (tbb::blocked_range2d<int, int> r)
  {
    for (int i = r.rows().begin(); i < r.rows().end(); i++) {
      for (int j = r.cols().begin(); j < r.cols().end(); j++) {
        mask[i][j] = (int)(matrix[i][j] > threshold);
      }
    }
  });
}

void TBB::thresh(int** matrix, int size, int percent, int** mask) {
  int nMax = 0;

  nMax = reduceMax(matrix, size);

  // Fill histogram
  int* histogram = new int[nMax];

  fillHistogram(matrix, size, histogram);

  int count = (size * size * percent) / 100;
  int prefixSum = 0;
  int threshold = nMax;

  for (int i = nMax; i >= 0 && prefixSum <= count; i--) {
    prefixSum += histogram[i];
    threshold = i;
  }

  // fill mask
  fillMask(matrix, size, threshold, mask);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      std::cout << mask[i][j] << " ";
    }
    std::cout << "\n";
  }
}