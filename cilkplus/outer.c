#include "cilkplus.h"

double sqr(double x) {

	return x * x;

}

double distance(struct point a, struct point b) {

	return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));

}

double max(double a, double b) {

	return a > b ? a : b;

}

void outer (struct point *points[]) {



}

