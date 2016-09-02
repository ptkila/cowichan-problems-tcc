#include "tbb/tbb.h"
#include <cfloat>
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
static Point* normPoints;
static Point* points;
static Point minPoint;
static Point maxPoint;
static int numThreads;

void findMaxMinPoints (const int size) {

  double maxX = -DBL_MAX;
  double maxY = -DBL_MAX;
  double minX = DBL_MAX;
  double minY = DBL_MAX;

  tbb::parallel_for(range(0, size), [&](const range& r) {
    for (size_t i = r.begin(); i != r.end(); i++) {
      for (int j = 0; j < size; j++) {
        if (points[i].x < minX) {
          minX = points[i].x;
        }

        if (points[i].y < minY) {
          minY = points[i].y;
        }

        if (points[i].x > maxX) {
          maxX = points[i].x;
        }

        if (points[i].y > maxY) {
          maxY = points[i].y;
        }
      }
    }
  });

  maxPoint.x = maxX;
  maxPoint.y = maxY;
  minPoint.x = minX;
  minPoint.y = minY;

}

void normalizePoints (const int size) {

  double sclX = (double)((maxPoint.x == minPoint.x) ?
    0.0 : 1.0 / (maxPoint.x - minPoint.x));

  double sclY = (double)((maxPoint.y == minPoint.y) ?
    0.0 : 1.0 / (maxPoint.y - minPoint.y));

  tbb::parallel_for(range(0, size), [&](const range& r) {
    for (size_t i = r.begin(); i != r.end(); i++) {
      normPoints[i].x = sclX * (points[i].x - minPoint.x);
      normPoints[i].y = sclY * (points[i].y - minPoint.y);
    }
   });
}

void norm (const int size) {

  findMaxMinPoints(size);
  normalizePoints(size);

} 

void setPointsValues(const int size) {
  for (int i = 0; i < size; ++i) {
    points[i].x = (double)(rand());
    points[i].y = (double)(rand());
  }
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

    points = new Point[size];
    normPoints = new Point[size];

    setThreadsNumber();
    setPointsValues(size);
    norm(size);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        std::cout << normPoints[i].x << " ";
        std::cout << normPoints[i].y << std::endl;
      }
    }

    delete[] points;
    delete[] normPoints;

  } else {


  }

  return 0;
}