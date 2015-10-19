#include "cpp11.h"

std::mutex m;

double sqr(double x) {

  return x * x;

}

double distance(class Point pointOne, class Point pointTwo) {

  // distncia euclidiana
  return sqrt(sqr(pointOne.x - pointOne.y) + sqr(pointTwo.x - pointTwo.y));

}

void doTheMath(double **matrix, int startLine, int endLine, double *vector, Point origin, Point *points, int numberOfPoints) {
  double nMax = -1.0;

  for (int i = startLine; i < endLine; i++) {
    for (int j = 0; j < numberOfPoints; j++) {
      m.lock();
      if (i != j) {
        matrix[i][j] = distance(points[i], points[j]);
        nMax = fmax(nMax, matrix[i][j]);
      }
      m.unlock();
    }
    matrix[i][i] = nMax * numberOfPoints;
    vector[i] = distance(origin, points[i]);
  }
}

void Cpp11::outer (Point *points, int numberOfPoints) {
  double **matrix;
  double *vector;
  int numThreads = 4;
  Point origin = Point(0, 0);
  int operationsByThread = numberOfPoints / numThreads;
  std::thread threadsList[numThreads];

  matrix = new double*[numberOfPoints];
  for (int i = 0; i < numberOfPoints; ++i)
    matrix[i] = new double[numberOfPoints];
  vector = new double[numberOfPoints];

  for (int i = 0; i < numThreads; ++i)
  {
    threadsList[i] = std::thread(doTheMath, matrix, operationsByThread * i, operationsByThread * (i + 1), vector, origin, points, numberOfPoints);
  }

  for (int i = 0; i < numThreads; ++i)
  {
    threadsList[i].join();
  }

  std::cout.precision(0);

  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfPoints; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }

  for (int i = 0; i < numberOfPoints; i++) {
    std::cout << vector[i] << " ";
  }

   std::cout << "\n";
}