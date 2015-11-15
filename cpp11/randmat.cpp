#include "cpp11.h"

const int VAL_A = 1313, VAL_B = 3131;

void doTheMath (const int startIndex, const int endIndex, std::vector<int>& matrix, const int givenSeed) {

	int seed = 0;

	for (int i = startIndex; i < endIndex; ++i) {

		seed = givenSeed + i;
		seed = VAL_A * seed + VAL_B;
		matrix[i] = seed % 100;

	}
}

void Cpp11::randmat(int nrows, int ncols, int s) {

	int totalMatrixSize = nrows * ncols;
	int numThreads = 4;
	int operationsByThread = totalMatrixSize / numThreads;

	std::vector<std::vector<int>> matrix(totalMatrixSize);
	std::vector<std::thread> threadsList;
	
	for (int i = 0; i < numThreads; ++i) {

		threadsList.push_back(std::thread(doTheMath, operationsByThread * i, operationsByThread * (i + 1), std::ref(matrix), s));

	}

	for ( auto &t : threadsList ) {

		t.join();

	}

	for (int i = 0; i < totalMatrixSize; ++i) {

		std::cout << matrix[i] << " ";
		if (i % ncols == 0 && i != 0) {
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}