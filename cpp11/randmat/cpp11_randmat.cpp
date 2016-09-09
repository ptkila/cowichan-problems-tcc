#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>

static const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;
static int numThreads;

void calc (const int startIndex, const int endIndex, const int size, const int seed) {

	int s = 0;
	for (int i = startIndex; i < endIndex; ++i) {
		for (int j = 0; j < size; j++) {
			s = VAL_A * (seed + i + j) + VAL_B;
			matrix[i*size + j] = s % 100;
		}
	}
}

void randmat (const int size, const int seed) {
	
	int numOpThreadM = (int)floor((float)size / (float)numThreads);
	int numOpThreadR = size % numThreads;
	std::thread threadsList[numThreads];
	
	for (int i = 0; i < numThreads; ++i) {
		if (i + 1 == numThreads && numOpThreadR > 0) {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size, seed));
			break;
		} else {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, 
				numOpThreadM * (i + 1), size, seed));
		}
	}

	for ( auto &t : threadsList ) {
		t.join();
	}

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size*size];
    randmat(size, rand());

    if (print == 1) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
        	std::cout << matrix[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
    }

    delete[] matrix;

  } else {


  }

  return 0;
}