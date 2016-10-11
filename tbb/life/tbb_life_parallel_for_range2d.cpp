#include "tbb/tbb.h"
#include <iostream>

typedef tbb::blocked_range2d<size_t, size_t> range2d;
static int* matrix;
static int* tmpMatrix;

int getNeighborValue(const int size, const int row, const int col) {

	if((row < 0 || row >= size) || (col < 0 || col >= size))
		return 0;
	return (matrix[row*size + col] == 1) ? 1 : 0;

}

int getNeighborCount(const int size, const int row, const int col) {

	int count = 0;

	count += getNeighborValue(size, row - 1, col - 1);
	count += getNeighborValue(size, row - 1, col);
	count += getNeighborValue(size, row - 1, col + 1);
	count += getNeighborValue(size, row, col - 1);
	count += getNeighborValue(size, row, col + 1);
	count += getNeighborValue(size, row + 1, col - 1);
	count += getNeighborValue(size, row + 1, col);
	count += getNeighborValue(size, row + 1, col + 1);

	return count;

}

void updateMatrix(const int size) {
	tbb::parallel_for(range2d(0, size, 0, size), [&](const range2d& r) -> void {
		std::size_t r_end = r.rows().end();
		for (std::size_t i = r.rows().begin(); i != r_end; ++i) {
			std::size_t c_end = r.cols().end();
			for (std::size_t j = r.cols().begin(); j != c_end; ++j) {
				matrix[i*size + j] = tmpMatrix[i*size + j];
			}
		}
	});
}

void evaluateMatrix (const int size) {
	tbb::parallel_for(range2d(0, size, 0, size), [&](const range2d& r) -> void {
		std::size_t r_end = r.rows().end();
		for (std::size_t i = r.rows().begin(); i != r_end; ++i) {
			std::size_t c_end = r.cols().end();
			for (std::size_t j = r.cols().begin(); j != c_end; ++j) {
				int count = getNeighborCount(size, i, j);
				if (matrix[i*size + j] == 1) {
					tmpMatrix[i*size + j] = (count == 2 || count == 3) ? 1 : 0;
				} else {
					tmpMatrix[i*size + j] = (count == 3) ? 1 : 0;
				}
			}
		}
	});
}

void play(const int size) {

	evaluateMatrix(size);
	updateMatrix(size);
		
}

void life (const int size, const int numgen) {

	for (int i = 0; i < numgen; ++i) {
		play(size);
	}

}

void setMatrixValues (const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 2;
		}
	}

}

void setThreadsNumber(const int numThreads) {

  tbb::task_scheduler_init init(numThreads);

}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size * size];
		tmpMatrix = new int[size * size];
		int numgen = 10000;

		setThreadsNumber(numThreads);
		setMatrixValues(size);
		life(size, numgen);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					std::cout << matrix[i*size + j] << " ";
				}
				std::cout << std::endl;
			}
		}

		delete[] matrix;
		delete[] tmpMatrix;

	} else {


	}

	return 0;
}