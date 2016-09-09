#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <mutex>

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

static Point* points;
static Point* normPoints;
static Point minPoint;
static Point maxPoint;
static int numThreads;

void findMinMaxPoints (const int startIndex, const int lastIndex) {

	for (int i = startIndex; i < lastIndex; ++i) {
		
		if (points[i].x < minPoint.x) {
			minPoint.x = points[i].x;
		}

		if (points[i].y < minPoint.y) {
			minPoint.y = points[i].y;
		}

		if (points[i].x > maxPoint.x) {
			maxPoint.x = points[i].x;
		}

		if (points[i].y > maxPoint.y) {
			maxPoint.y = points[i].y;
		}
	
	}
}

void normalizePoints (const int startIndex, const int lastIndex, const double sclX, 
	const double sclY) {

	for (int i = startIndex; i < lastIndex; ++i) {

		normPoints[i].x = sclX * (points[i].x - minPoint.x);
		normPoints[i].y = sclY * (points[i].y - minPoint.y);

	}
}

void norm (const int size){

	int numOpThreadM = (int)floor((float)size/ (float)numThreads);
	int numOpThreadR = size % numThreads;  
	std::thread threadsList[numThreads];

	for (int i = 0; i < numThreads; ++i) {
		if  (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(findMinMaxPoints, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR);
			break;
		} else {
			threadsList[i] = std::thread(findMinMaxPoints, numOpThreadM * i, 
				numOpThreadM * (i + 1));
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}

	//std::cout << "Max = " << maxPoint.x << " " << maxPoint.y << std::endl;
	//std::cout << "Min = " << minPoint.x << " " << minPoint.y << std::endl;

	double sclX = (double)((maxPoint.x == minPoint.x) ?
		0.0 : 1.0 / (maxPoint.x - minPoint.x));

	double sclY = (double)((maxPoint.y == minPoint.y) ?
		0.0 : 1.0 / (maxPoint.y - minPoint.y));

	for (int i = 0; i < numThreads; ++i) {
		if  (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(normalizePoints, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, sclX, sclY);
			break;
		} else {
			threadsList[i] = std::thread(normalizePoints, numOpThreadM * i, 
				numOpThreadM * (i + 1), sclX, sclY);
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}

}

void setPointsValues (const int size) {
	for (int i = 0; i < size; ++i) {
		points[i].x = (double)(std::rand());
		points[i].y = (double)(std::rand());
	}
	/*
	for (int i = 0; i < size; i++) {
		std::cout << points[i].x << " ";
		std::cout << points[i].y << "\n" << std::endl;
	}
	*/
	
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));

		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		points = new Point[size];
		normPoints = new Point[size];

		double infP = std::numeric_limits<double>::max();
		double infM = std::numeric_limits<double>::lowest();

		maxPoint = Point(infM, infM);
		minPoint = Point(infP, infP);

		setPointsValues(size);
		norm(size);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				std::cout << normPoints[i].x << " ";
				std::cout << normPoints[i].y << std::endl;
			}
		}

		delete[] points;
		delete[] normPoints;

	} else {


	}

	return 0;
}