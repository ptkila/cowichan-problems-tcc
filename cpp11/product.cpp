#include "cpp11.h"

void doTheMath (double **matrix, double *vector, int startIndex, int lastIndex, int size, double** result) {
	for (int i = startIndex; i < lastIndex; i++) {
		for (int j = 0; j < size; j++) {
			result[i][j] += matrix[i][j] * vector[j];
		}
	}
}

void Cpp11::product (double **matrix, double *vector, int size) {

	int numOfThreads = 4;
	int operationsByThread = size / numOfThreads;
	double** result = new double*[size];
	std::thread threadsList[numOfThreads];

	for (int i = 0; i < size; ++i) {
		result[i] = new double[size];
	}

	for (int i = 0; i < numOfThreads; ++i) {
		threadsList[i] = std::thread(doTheMath, matrix, vector, operationsByThread * i, operationsByThread * (i + 1), size,
		                             result);
	}

	for (int i = 0; i < numOfThreads; ++i) {

		threadsList[i].join();

	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << result[i][j] << " ";
		}
		std::cout << "\n";
	}

}