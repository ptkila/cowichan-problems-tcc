#include "cpp11.h"

int main () {

	Cpp11 *cpp = new Cpp11();

	//Teste randmat
	//cpp->randmat(2, 2, 10);

	//Teste norm
	int numberOfPoints = 1000000;
	Point *points = new Point[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {

		points[i].x = (double)(i);
		points[i].y = (double)(i);

	}
	cpp->norm(points, numberOfPoints);

	return 0;
}