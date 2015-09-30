#include "cpp11.h"

void findMinMaxPoints (Point *points, int numberOfPoints, Point *minPoint, Point *maxPoint) {

  for (int i = 1; i < numberOfPoints; i++) {

    if (points[i].x < minPoint->x) {
      minPoint->x = points[i].x;
    }

    if (points[i].y < minPoint->y) {
      minPoint->y = points[i].y;
    }

    if (points[i].x > maxPoint->x) {
      maxPoint->x = points[i].x;
    }

    if (points[i].y > maxPoint->y) {
      maxPoint->y = points[i].y;
    }
  }
}

void normalizePoints (Point points[], int startIndex, int lastIndex, Point *minPoint, Point *maxPoint, double sclX, double sclY, Point normPoints[]) {

  for (int i = startIndex; i < lastIndex; ++i) {

    normPoints[i].x = sclX * (points[i].x - minPoint->x);
    normPoints[i].y = sclY * (points[i].y - minPoint->y);

  }
}

void Cpp11::norm (Point *points, int numberOfPoints) {

  Point *normPoints = new Point[numberOfPoints];
  Point *minPoint = new Point();
  Point *maxPoint = new Point();
  int numOfThreads = 4;
  int operationsByThread = numberOfPoints / numOfThreads;
  std::thread threadList[numOfThreads];
  double sclX = 0, sclY = 0;

  findMinMaxPoints(points, numberOfPoints, minPoint, maxPoint);

  sclX = (double)((maxPoint->x == minPoint->x) ?
                  0.0 : 1.0 / (maxPoint->x - minPoint->x));

  sclY = (double)((maxPoint->y == minPoint->y) ?
                  0.0 : 1.0 / (maxPoint->y - minPoint->y));

  for (int i = 0; i < numOfThreads; ++i) {

    threadList[i] = std::thread(normalizePoints, points, operationsByThread * i, operationsByThread * (i + 1), minPoint, maxPoint, sclX, sclY, normPoints);

  }

  for (int i = 0; i < numOfThreads; ++i) {

    threadList[i].join();

  }

  for (int i = 0; i < numberOfPoints; i++) {

    std::cout << normPoints[i].x << " " << normPoints[i].y << endl;

  }
}