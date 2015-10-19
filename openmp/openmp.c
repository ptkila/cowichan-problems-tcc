#include "openmp.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main () {

	//randmat(2, 2, 10);
	
	// Teste norm
	/*
	int number_of_points = 3;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	norm(points, number_of_points);
	*/
	
	// Teste outer
	int number_of_points = 4;
	struct point *points = (struct point*) malloc(sizeof(struct point) * number_of_points);
	int i = 0;
	srand (time(NULL));
	
	for (i = 0; i < number_of_points; i++) {

		points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
		points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
	
	}

	outer(points, number_of_points);

	return 0;

}