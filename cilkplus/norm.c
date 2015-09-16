#include "cilkplus.h"

void find_min_max_points (struct point *points, int number_of_points, struct point* min_point,
    struct point* max_point) {

	int i = 0;
  	max_point->x = points[0].x;
  	max_point->y = points[0].y;
  	min_point->x = points[0].x;
  	min_point->y = points[0].y;

  	for (i = 1; i < number_of_points; i++) {

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

void normalize_points (struct point *points, int number_of_points, struct point min_point, struct point max_point, struct point *norm_points) {

	double sclX = 0, sclY = 0;
	int i = 0;

	// x = (xi - xmin) * (1 / (xmax - xmin))
	sclX = (double)((max_point.x == min_point.x) ?
      0.0 : 1.0 / (max_point.x - min_point.x));
  	
  	sclY = (double)((max_point.y == min_point.y) ?
      0.0 : 1.0 / (max_point.y - min_point.y));

  	for (i = 0; i < number_of_points; i++) {
 
    	norm_points[i].x = sclX * (points[i].x - min_point.x);
    	norm_points[i].y = sclY * (points[i].y - min_point.y);
 
  	}
}

void norm (struct point* points, int number_of_points) {

	struct point *norm_points = (struct point*) malloc(sizeof(struct point)*number_of_points);
	struct point min_point;
	struct point max_point;
	int i = 0;

  	find_min_max_points(points, number_of_points, &min_point, &max_point);
  	normalize_points(points, number_of_points, min_point, max_point, norm_points);

  	for (i = 0; i < number_of_points; i++) {
 
    	printf("%f ", norm_points[i].x);
    	printf("%f\n", norm_points[i].y);
 
  	}
}