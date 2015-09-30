#include "cilkplus.h"

int main () {

	//randmat(2, 2, 10);

	int number_of_points = 8;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	norm(points, number_of_points);
	return 0;

}