#include "cpp11.h"

void find_min_max_points (Point *points, int numberOfPoints, Point *min_point,
    Point *max_point);
void normalize_points (Point *points, int numberOfPoints, Point min_point, Point max_point, Point *norm_points);

void Cpp11::norm (Point *points, int numberOfPoints){

	Point *norm_points = new Point[numberOfPoints];
	Point *min_point = new Point();
	Point *max_point = new Point();

  	find_min_max_points(points, numberOfPoints, &min_point, &max_point);
  	normalize_points(points, numberOfPoints, min_point, max_point, norm_points);

  	for (int i = 0; i < numberOfPoints; i++) {
 
    	printf("%f ", norm_points[i].x);
    	printf("%f\n", norm_points[i].y);
 
  	}

}

void find_min_max_points (Point *points, int numberOfPoints, Point *min_point,
    Point *max_point) {

  	max_point->x = points[0].x;
  	max_point->y = points[0].y;
  	min_point->x = points[0].x;
  	min_point->y = points[0].y;

  	for (int i = 1; i < numberOfPoints; i++) {

    	if (points[i].x < min_point->x) {
      		min_point->x = points[i].x;
    	}
    
    	if (points[i].y < min_point->y) {
      		min_point->y = points[i].y;
    	}
    
    	if (points[i].x > max_point->x) {
      		max_point->x = points[i].x;
    	}
    
    	if (points[i].y > max_point->y) {
      		max_point->y = points[i].y;
    	}
  	}
}

void normalize_points (Point *points, int numberOfPoints, Point min_point, Point max_point, Point *norm_points) {

	double sclX = 0, sclY = 0;

	// x = (xi - xmin) * (1 / (xmax - xmin))
	sclX = (double)((max_point.x == min_point.x) ?
      0.0 : 1.0 / (max_point.x - min_point.x));
  	
  	sclY = (double)((max_point.y == min_point.y) ?
      0.0 : 1.0 / (max_point.y - min_point.y));

  	for (int i = 0; i < numberOfPoints; i++) {
 
    	norm_points[i].x = sclX * (points[i].x - min_point.x);
    	norm_points[i].y = sclY * (points[i].y - min_point.y);
 
  	}
}