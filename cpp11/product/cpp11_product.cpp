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
static int* result;
static int* vector;

void calc(const int startIndex, const int lastIndex, const int size) {
	for (int i = startIndex; i < lastIndex; i++) {
		for (int j = 0; j < size; j++) {
			result[i * size + j] += matrix[i * size + j] * vector[j];
		}
	}
}

void product(const int size, const int numberOfThreads) {

	int numOpThreadM = (int)floor((float)size / (float)numberOfThreads);
	int numOpThreadR = size % numberOfThreads;
	std::thread threadsList[numberOfThreads];

	for (int i = 0; i < numberOfThreads; ++i) {
		if (i + 1 == numberOfThreads && numOpThreadR > 0) {
			threadsList[i] = (std::thread(calc, numOpThreadM * i,  numOpThreadM * (i + 1) + numOpThreadR, size));
			break;
		} else {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1), size));
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}

}

void setValuesMatrix(const int size) {

  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[i * size + j] = rand() % 100;    
    }
  }
}

void setValuesVector(const int size) {

  int i;
  for (i = 0; i < size; ++i) {
    vector[i] = rand() % 100;
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int i, j;

    matrix = new int[size * size];
    result = new int[size * size];
    vector = new int[size];

    setValuesMatrix(size);
    setValuesVector(size);
    product(size, numThreads);

    if (print == 1) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				std::cout << result[i * size + j] << " ";
			}
			std::cout << "\n";
		}
	}
  } else {


  }

  return 0;
}