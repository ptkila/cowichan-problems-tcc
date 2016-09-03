#include "tbb/tbb.h"
#include "tbb/blocked_range2d.h"
#include <iostream>

typedef tbb::blocked_range2d<size_t, size_t> range2d;
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

void mandel (const int size) {
	
	const double x0 = ((double)rand()/ (double)RAND_MAX);
	const double y0 = ((double)rand()/ (double)RAND_MAX);
	const double dx = ((double)rand()/ (double)RAND_MAX)/ (double)(size - 1);
	const double dy = ((double)rand()/ (double)RAND_MAX)/ (double)(size - 1);

	tbb::parallel_for(range2d(0, size, 0, size), [&](const range2d& r) {
		size_t r_end = r.rows().end();
		for (size_t i = r.rows().begin(); i != r_end; i++) {
			size_t c_end = r.cols().end();
			for (size_t j = r.cols().begin(); j != c_end; j++) {
				matrix[i*size + j] = calc(x0 + ((double)i * dx), y0 + ((double)j * dy));
			}
		}
	});
}

void setThreadsNumber() {

	tbb::task_scheduler_init init(numThreads);

}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size*size];

		setThreadsNumber();
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