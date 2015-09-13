#include "cpp11.hpp"

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
	
	int num_threads = 0;
	int operationsByThread = 0;
	if (nrows >= ncols) {
		num_threads = ncols;
		operationsByThread = nrows;
	} else {
		num_threads =  nrows;
		operationsByThread = ncols;
	}
	std::thread threads_list[num_threads];

	for (int i = 0; i < num_threads; ++i) {

		threads_list[i] = std::thread(doTheMath, operationsByThread * i, operationsByThread * (i + 1), matrix, s);

	}

	for (int i = 0; i < num_threads; ++i) {

		threads_list[i].join();

	}

  	for (int i = 0; i < totalMatrixSize; ++i) {
    	 	
    	printf("%d ", matrix[i]);    
    	
    }
}