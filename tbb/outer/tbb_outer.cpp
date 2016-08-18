#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib> 
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"

class Point {

public:

  double x;
  double y;

  Point () {
    this->x = 0.0;
    this->y = 0.0;
  }

  Point (double x, double y) {
    this->x = x;
    this->y = x;
  }
};

typedef tbb::blocked_range<size_t> range;
static Point* points;
static double* matrix;
static double* vector;

double sqr(const double x) {

  return x * x;

}

double distance(const class Point a, const class Point b) {

  return sqrt(sqr(a.x - a.y) + sqr(b.x - b.y));

}

void outer(const int size) {
  parallel_for(
    range(0, size),
    [&](range r) {
      auto begin = r.begin();
      auto end = r.end();
      for (size_t i = begin; i != end; ++i) {
        double nmax = -1;
        for (int j = 0; j < size; j++) {
          if (i != j) {
            matrix[i*size + j] = distance(points[i], points[j]);
            nmax = std::fmax(nmax, matrix[i*size + j]);
          }
        }
        matrix[i*size + i] = size * nmax;
        vector[i] = distance(new Point(), points[i]);
      }
    });
}

void setPointValues(const int size) {
  for (int i = 0; i < size; i++) {
    points[i].x = rand();
    points[i].y = rand();
  }
}

void setThreadsNumber(const int threadsNumber) {

  tbb::task_scheduler_init init(threadsNumber);

}

int main(int argc, char** argv) {

  if (argc == 4)
  {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new double[size * size];
    vector = new double[size];
    points = new Point[size];

    setPointValues(size);
    setThreadsNumber(num_threads);
    outer(size);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%g ", matrix[i*size + j]);
        }
        printf("\n");
      }
      printf("\n");

      for (int i = 0; i < size; i++) {
        printf("%g ", vector[i]);
      }
      printf("\n");
    }
  } else {



  }
  return 0;
}