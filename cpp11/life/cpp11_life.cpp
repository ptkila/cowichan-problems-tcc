#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>

static int* matrix;
static int* tmpMatrix;
static int numgen;
static int numThreads; 

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

void updateMatrix(const int startIndex, const int lastIndex, const int size) {
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

void play(const int size) {

	int numOpThreadM = (int)floor((float)size/ (float)numThreads);
	int numOpThreadR = size % numThreads;  
	std::thread threadsList[numThreads];

	for (int i = 0; i < numThreads; ++i) {
		if (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(evaluateMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size);
			break;
		} else {
			threadsList[i] = std::thread(evaluateMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1), size);
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}	

	for (int i = 0; i < numThreads; ++i) {
		if  (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = std::thread(updateMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size);
			break;
		} else {
			threadsList[i] = std::thread(updateMatrix, numOpThreadM * i, 
				numOpThreadM * (i + 1), size);
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}
	
	/*
	for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				printf("%d ", matrix[i*size + j]);
			}
			printf("\n");
	}
	printf("\n");
	*/	
}

void life (const int size, const int numgen) {

	for (int i = 0; i < numgen; i++) {
		play(size);
	}
	
}

void setMatrixValues (const int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i*size + j] = rand() % 2;
		}
	}
	/*
	for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				printf("%d ", matrix[i*size + j]);
			}
			printf("\n");
	}
	printf("\n");
	*/
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size * size];
		tmpMatrix = new int[size * size];
		numgen = 5;

		setMatrixValues(size);
		life(size, numgen);

		if (print == 1) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
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