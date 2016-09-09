#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cmath>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <future>

static int* matrix;
static int* vector;
static int* result;
static int numThreads;

void calc(const int startIndex, const int lastIndex, const int size) {
	for (int i = startIndex; i < lastIndex; ++i) {
		int sum = 0;
		for (int j = 0; j < size; ++j) {
			sum += matrix[i*size + j] * vector[j];
		}
		result[i] = sum;
	}
}

void product(const int size) {

	int numOpThreadM = (int)floor((float)size / (float)numThreads);
	int numOpThreadR = size % numThreads;
	std::thread threadsList[numThreads];

	for (int i = 0; i < numThreads; ++i) {
		if (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size));
			break;
		} else {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, 
				numOpThreadM * (i + 1), size));
		}
	}

	for (auto &t : threadsList) {
		t.join();
	}

}

void setValuesMatrix(const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 100;    
		}
	}
}

void setValuesVector(const int size) {
	for (int i = 0; i < size; ++i) {
		vector[i] = rand() % 100;
	}
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size * size];
		result = new int[size];
		vector = new int[size];

		setValuesMatrix(size);
		setValuesVector(size);
		product(size);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
		
				std::cout << result[i] << " ";
		
			}
		}

		delete[] matrix;
		delete[] vector;
		delete[] result;

	} else {


	}

	return 0;
}