#include "cpp11.h"

void calc (int startIndex, int lastIndex, double **matrix, double *vector, int size, double** result) {
	for (int i = startIndex; i < lastIndex; i++) {
		for (int j = 0; j < size; j++) {
			result[i][j] += matrix[i][j] * vector[j];
		}
	}
}

double** Cpp11::product (double **matrix, double *vector, int size, int numberOfThreads, int shouldPrint) {

	int numOpThreadM = (int)floor((float)size / (float)numberOfThreads);
	int numOpThreadR = size % numberOfThreads;
	std::thread threadsList[numberOfThreads];

	double** result = new double*[size];
	for (int i = 0; i < size; ++i) {
		result[i] = new double[size];
	}

	for (int i = 0; i < numberOfThreads; ++i) {
		if (i + 1 == numberOfThreads && numOpThreadR > 0) {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * i + numOpThreadR + numOpThreadM, matrix, vector, size, result));
			break;
		} else {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1), matrix, vector, size, result));
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}

	if (shouldPrint == 1) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				std::cout << result[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

}