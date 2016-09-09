#include "tbb/tbb.h"
#include <iostream>

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
static int numThreads;

double sqr(const double x) {

  return x * x;

}

double distance(const Point a, const Point b) {

  return sqrt(sqr(a.x - a.y) + sqr(b.x - b.y));

}

void outer(const int size) {
  tbb::parallel_for(range(0, size),[&](const range& r) {
    size_t r_end = r.end();
    for (size_t i = r.begin(); i != r_end; ++i) {
        double nmax = 0;
        for (int j = 0; j < size; ++j) {
          if (i != j) {
            matrix[i*size + j] = distance(points[i], points[j]);
            nmax = std::fmax(nmax, matrix[i*size + j]);
          }
        }
        matrix[i*size+i] = size * nmax;
        vector[i] = distance(Point(), points[i]);
      }
  });
}

void setPointValues(const int size) {
  for (int i = 0; i < size; ++i) {
    points[i].x = rand();
    points[i].y = rand();
  }
}

void setThreadsNumber() {

  tbb::task_scheduler_init init(numThreads);

}

int main(int argc, char** argv) {

  if (argc == 4)
  {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new double[size * size];
    vector = new double[size];
    points = new Point[size];

    setThreadsNumber();
    setPointValues(size);
    outer(size);

    if (print == 1) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          std::cout << matrix[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;

      for (int i = 0; i < size; ++i) {
        std::cout << vector[i] << " ";
      }
      std::cout << std::endl;
    }

    delete[] matrix;
    delete[] vector;
    delete[] points;

  } else {


  }
  return 0;
}