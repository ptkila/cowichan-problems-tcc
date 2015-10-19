#include "tbb.h"

double sqr(double x) {

  return x * x;

}

double distance(class Point pointOne, class Point pointTwo) {

  // distncia euclidiana
  return sqrt(sqr(pointOne.x - pointOne.y) + sqr(pointTwo.x - pointTwo.y));

}

void doTheMath(double **matrix, double *vector, Point origin, Point *points, int numberOfPoints) {
  tbb::parallel_for(tbb::blocked_range2d<int, int> (0, numberOfPoints, 0, 0, numberOfPoints, 0), [ = ] (tbb::blocked_range2d<int, int> r)
  {
    double nMax = -1.0;
    for (int i = r.rows().begin(); i < r.rows().end(); i++) {
      for (int j = r.cols().begin(); j < r.cols().end(); j++) {
        if (i != j) {
          matrix[i][j] = distance(points[i], points[j]);
          nMax = fmax(nMax, matrix[i][j]);
        }
      }
      matrix[i][i] = nMax * numberOfPoints;
      vector[i] = distance(origin, points[i]);
    }
  });
}

void TBB::outer (class Point *points, int numberOfPoints) {
  double **matrix;
  double *vector;
  Point origin = Point(0, 0);

  matrix = new double*[numberOfPoints];
  for (int i = 0; i < numberOfPoints; ++i)
    matrix[i] = new double[numberOfPoints];
  vector = new double[numberOfPoints];

  doTheMath(matrix, vector, origin, points, numberOfPoints);

  std::cout.precision(2);

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