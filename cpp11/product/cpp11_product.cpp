#include <iostream>
#include "../ThreadPool.h"

static double* matrix;
static double* vector;
static double* result;

void fillValues(const int startIndex, const int lastIndex, const int size) {
	for (int i = startIndex; i < lastIndex; ++i) {
		double sum = 0.0;
		for (int j = 0; j < size; ++j) {
			sum += matrix[i*size + j] * vector[j];
		}
		result[i] = sum;
	}
}

void product(const int size, const int numThreads) {

	ThreadPool pool(numThreads);
	pool.parallel_for(fillValues, size);
	pool.waitAll();

}

void setValuesMatrix(const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 10;    
		}
	}
}

void setValuesVector(const int size) {
	for (int i = 0; i < size; ++i) {
		vector[i] = rand() % 10;
	}
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new double[size * size];
		result = new double[size];
		vector = new double[size];

		setValuesMatrix(size);
		setValuesVector(size);
		product(size, numThreads);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				std::cout << result[i] << " ";
			}
			std::cout << std::endl;
		}

		delete[] matrix;
		delete[] vector;
		delete[] result;

	} else {


	}

	return 0;
}