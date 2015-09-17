#include "tbb.h"

 void findMinMaxPoints (Point *points, int numberOfPoints, Point *minPoint, Point *maxPoint) {

    tbb::parallel_for(tbb::blocked_range<int>(0, numberOfPoints), [=](tbb::blocked_range<int> r) {
      auto end = r.end (); 
      for (int i = r.begin(); i != end; ++i) {
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
    });

}

void normalizePoints (Point points[], int numberOfPoints, Point *minPoint, Point *maxPoint, double sclX, double sclY, Point normPoints[]) {

    tbb::parallel_for(tbb::blocked_range<int>(0, numberOfPoints), [=](tbb::blocked_range<int> r) {
      auto end = r.end (); 
      for (int i = r.begin(); i != end; ++i) {
        normPoints[i].x = sclX * (points[i].x - minPoint->x);
        normPoints[i].y = sclY * (points[i].y - minPoint->y); 
      }
    });
}

void TBB::norm (Point *points, int numberOfPoints){

	Point *normPoints = new Point[numberOfPoints];
	Point *minPoint = new Point();
	Point *maxPoint = new Point();
	double sclX = 0, sclY = 0;

  findMinMaxPoints(points, numberOfPoints, minPoint, maxPoint);

  sclX = (double)((maxPoint->x == minPoint->x) ?
      0.0 : 1.0 / (maxPoint->x - minPoint->x));
  sclY = (double)((maxPoint->y == minPoint->y) ?
      0.0 : 1.0 / (maxPoint->y - minPoint->y));

  normalizePoints(points, numberOfPoints, minPoint, maxPoint, sclX, sclY, normPoints);

  for (int i = 0; i < numberOfPoints; i++) {
 
    std::cout << normPoints[i].x << " " << normPoints[i].y << endl;
 
  }
}