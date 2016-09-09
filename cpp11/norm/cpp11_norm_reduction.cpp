#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <future>

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

class MinMaxPoints {

public:
	
	double minX;
	double minY;
	double maxX;
	double maxY;

	MinMaxPoints () {

		double infP = std::numeric_limits<double>::max();
		double infM = std::numeric_limits<double>::lowest();
		this->minX = this->minY = infP;
		this->maxX = this->maxY = infM;
	
	}

	static void compareAndSet(MinMaxPoints& f, MinMaxPoints& s) {

		if (f.minX > s.minX) {
			f.minX = s.minX;
		}

		if (f.minY > s.minY) {
			f.minY = s.minY;
		}

		if (f.maxX < s.maxX) {
			f.maxX = s.maxX;
		}

		if (f.maxY < s.maxY) {
			f.maxY = s.maxY;
		}
	}
};

static Point* points;
static Point* normPoints;
static MinMaxPoints minMaxPoints;
static int numThreads;

void findMinMaxPoints (const int startIndex, const int lastIndex, 
	std::promise<MinMaxPoints>* promise) {

	MinMaxPoints p = MinMaxPoints();

	for (int i = startIndex; i < lastIndex; ++i) {
		
		if (points[i].x < p.minX) {
			p.minX = points[i].x;
		}

		if (points[i].y < p.minY) {
			p.minY = points[i].y;
		}

		if (points[i].x > p.maxX) {
			p.maxX = points[i].x;
		}

		if (points[i].y > p.maxY) {
			p.maxY = points[i].y;
		}
	}

	promise->set_value(p);

}

void normalizePoints (const int startIndex, const int lastIndex, const double sclX, const double sclY) {

	for (int i = startIndex; i < lastIndex; ++i) {

		normPoints[i].x = sclX * (points[i].x - minMaxPoints.minX);
		normPoints[i].y = sclY * (points[i].y - minMaxPoints.minY);

	}
}

void norm (const int size){

	int numOpThreadM = (int)floor((float)size/ (float)numThreads);
	int numOpThreadR = size % numThreads;  
	std::thread threadsList[numThreads];
	std::promise<MinMaxPoints> promises[numThreads];
  	std::future <MinMaxPoints> futures[numThreads]; 

	for (int i = 0; i < numThreads; ++i) {
		futures[i] = promises[i].get_future();
		if  (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(findMinMaxPoints, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, &promises[i]);
			break;
		} else {
			threadsList[i] = std::thread(findMinMaxPoints, numOpThreadM * i, 
				numOpThreadM * (i + 1), &promises[i]);
		}
	}

	for (int i = 0; i < numThreads; ++i) {

    	MinMaxPoints pTmp = futures[i].get();
    	MinMaxPoints::compareAndSet(minMaxPoints, pTmp);
  	
  	}

	for ( auto &t : threadsList ) {
		t.join();
	}

	//std::cout << "Max = " << minMaxPoints.maxX << " " << minMaxPoints.maxY << std::endl;
	//std::cout << "Min = " << minMaxPoints.minX << " " << minMaxPoints.minY << std::endl;

	double sclX = (double)((minMaxPoints.maxX == minMaxPoints.minX) ?
		0.0 : 1.0 / (minMaxPoints.maxX - minMaxPoints.minX));

	double sclY = (double)((minMaxPoints.maxY == minMaxPoints.minY) ?
		0.0 : 1.0 / (minMaxPoints.maxY - minMaxPoints.minY));

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

	delete[] promises;
	delete[] futures;

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
		minMaxPoints = MinMaxPoints();

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