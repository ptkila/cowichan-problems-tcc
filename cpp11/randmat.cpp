#include "cpp11.h"

//static const int num_threads = 4;
const int VAL_A = 1313, VAL_B = 3131;

void doTheMath (int startIndex, int endIndex, int matrix[], int givenSeed) {

	int seed = 0;

	for (int i = startIndex; i < endIndex; ++i) {

		seed = givenSeed + i;
		seed = VAL_A * seed + VAL_B;
		matrix[i] = seed % 100;

	}
}

void Cpp11::randmat(int nrows, int ncols, int s) {

	int totalMatrixSize = nrows * ncols;
	int *matrix = new int[totalMatrixSize];
	int numThreads = 4;
	int operationsByThread = nrows * ncols / numThreads;
	std::thread threadsList[numThreads];

	for (int i = 0; i < numThreads; ++i) {

		threadsList[i] = std::thread(doTheMath, operationsByThread * i, operationsByThread * (i + 1), matrix, s);

	}

	for (int i = 0; i < numThreads; ++i) {

		threadsList[i].join();

	}

	for (int i = 0; i < totalMatrixSize; ++i) {

		std::cout << matrix[i] << " ";
		if (i % ncols == 0 && i != 0) {
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}