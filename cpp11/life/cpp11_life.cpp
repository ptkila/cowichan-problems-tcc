#include <iostream>
#include "../ThreadPool.h"

static int* matrix;
static int* tmpMatrix;

void printMatrix(const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << matrix[i*size + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int getNeighborValue(const int size, const int row, const int col) {

	if((row < 0 || row >= size) || (col < 0 || col >= size)){
		return 0;
	} else {
		return (matrix[row*size + col] == 1) ? 1 : 0;
	}
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

void updateMatrix (const int startIndex, const int lastIndex, const int size) {
	for (int i = startIndex; i < lastIndex; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = tmpMatrix[i*size + j];
		}
	}
}

void evaluateMatrix (const int startIndex, const int lastIndex, const int size) {
	int count = 0;
	for (int i = startIndex; i < lastIndex; ++i) {
		for (int j = 0; j < size; ++j) {
			count = getNeighborCount(size, i, j);
			if (matrix[i*size + j] == 1) {
				tmpMatrix[i*size + j] = (count == 2 || count == 3) ? 1 : 0;
			} else {
				tmpMatrix[i*size + j] = (count == 3) ? 1 : 0;
			}
		}
	}
}

void play(const int size, ThreadPool& pool) {  
	
	pool.parallel_for(evaluateMatrix, size);
	pool.waitAll();

	pool.parallel_for(updateMatrix, size);
	pool.waitAll();	

}

void life (const int size, const int numgen, const int numThreads) {
	
	ThreadPool pool(numThreads);
	
	for (int i = 0; i < numgen; ++i) {
		//printMatrix(size);
		play(size, pool);
	}
}

void setMatrixValues (const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 2;
		}
	}
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

		setMatrixValues(size);
		life(size, numgen, numThreads);

		if (print == 1) {
			printMatrix(size);
		}

		delete[] matrix;
		delete[] tmpMatrix;

	} else {


	}

	return 0;
}