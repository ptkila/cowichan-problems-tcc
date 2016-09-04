#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>

static const int MAX_STEPS = 150;
static int* matrix;
static int numThreads;

int calc (double x, double y) {
	int iter;
	double xx;
	for (iter = 0; iter < MAX_STEPS; iter++)  {
		xx = x*x - y*y;
		y = 2.0 * x * y;
		x = xx;
		if ((x*x + y*y) >= 2.0) {
			return iter;
		}
	}
	return MAX_STEPS;
}

void fillMatrix(const int startIndex, const int endIndex, const int size, const double x0,
 const double y0, const double dx, const double dy) {
	for (int i = startIndex; i < endIndex; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i*size + j] = calc(x0 + ((double)i * dx), y0 + ((double)j * dy));
		}
	}
}

void mandel (const int size) {
	
	const double x0 = ((double)rand()/ (double)RAND_MAX);
	const double y0 = ((double)rand()/ (double)RAND_MAX);
	const double dx = ((double)rand()/ (double)RAND_MAX)/ (double)(size - 1);
	const double dy = ((double)rand()/ (double)RAND_MAX)/ (double)(size - 1);

	int numOpThreadM = (int)floor((float)size/ (float)numThreads);
	int numOpThreadR = size % numThreads;  
	std::thread threadsList[numThreads];

	for (int i = 0; i < numThreads; ++i) {
		if  (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(fillMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size, x0, y0, dx, dy);
			break;
		} else {
			threadsList[i] = std::thread(fillMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1), size, x0, y0, dx, dy);
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size*size];

		mandel(size);

		if (print == 1) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					std::cout << matrix[i*size + j] << " ";
				}
				std:: cout << std::endl;
			}
		}

		delete[] matrix;

	} else {


	}

	return 0;
}