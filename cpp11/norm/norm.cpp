#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cmath>
#include <thread>

class Point {

public:

	double x;
	double y;

	Point () {
		this->x = 0.0;
		this->y = 0.0;
	}

	Point (double x, double y) {
		this->x = x;
		this->y = x;
	}
};

Point* points;
Point* normPoints;

void findMinMaxPoints (int numberOfPoints, Point *minPoint, Point *maxPoint) {

	for (int i = 1; i < numberOfPoints; i++) {

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
}

void normalizePoints (int startIndex, int lastIndex, Point *minPoint, Point *maxPoint, double sclX, double sclY, int tN) {

/*
	printf("Thread Number: %d\n", tN);
	printf("St = %d\n", startIndex);
	printf("Et = %d\n", lastIndex);
*/
	for (int i = startIndex; i < lastIndex; ++i) {

		normPoints[i].x = sclX * (points[i].x - minPoint->x);
		normPoints[i].y = sclY * (points[i].y - minPoint->y);

	}
}

void norm (int numberOfPoints, int numberOfThreads){

	int numOpThreadM = (int)floor((float)numberOfPoints / (float)numberOfThreads);
	int numOpThreadR = numberOfPoints % numberOfThreads;  
	double sclX = 0, sclY = 0;
	Point *normPoints = new Point[numberOfPoints];
	Point *minPoint = new Point();
	Point *maxPoint = new Point();
	std::thread threadsList[numberOfThreads];

	findMinMaxPoints(numberOfPoints, minPoint, maxPoint);

	sclX = (double)((maxPoint->x == minPoint->x) ?
		0.0 : 1.0 / (maxPoint->x - minPoint->x));

	sclY = (double)((maxPoint->y == minPoint->y) ?
		0.0 : 1.0 / (maxPoint->y - minPoint->y));

	for (int i = 0; i < numberOfThreads; ++i) {
		if  (i + 1 == numberOfThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(normalizePoints, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, minPoint, maxPoint, sclX, sclY, i);
			break;
		} else {
			threadsList[i] = std::thread(normalizePoints, numOpThreadM * i, numOpThreadM * (i + 1), 
				minPoint, maxPoint, sclX, sclY, i);
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}
}

void setPointsValues (int size) {
	for (int i = 0; i < size; i++) {
		points[i].x = (double)(std::rand());
		points[i].y = (double)(std::rand());
	}
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		points = new Point[size];
		normPoints = new Point[size];

		setPointsValues(size);
		norm(size, numThreads);

		if (print == 1) {
			for (int i = 0; i < size; i++) {
				printf("%f ", normPoints[i].x);
				printf("%f\n", normPoints[i].y);
			}
		}

	} else {


	}

	return 0;
}