#include "tbb.h"

int main () {

	TBB *tbb = new TBB();
	//teste randmat
	//tbb->randmat(2,2,10);
	
	//teste norm
	int numberOfPoints = 8;
	Point *points = new Point[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);
	
	}
	tbb->norm(points, numberOfPoints);
	return 0;
	
}