#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cmath>
#include <thread>

const int VAL_A = 1313, VAL_B = 3131;
static int* matrix;

void calc (const int startIndex, const int endIndex, const int size, const int seed) {

	int s = 0;

	for (int i = startIndex; i < endIndex; ++i) {
		for (int j = 0; j < size; ++j) {
			s = VAL_A * (seed + i + j) + VAL_B;
			matrix[i*size + j] = seed % 100;
		}
	}
}

void randmat (int size, int s, int numberOfThreads) {
	
	int numOpThreadM = (int)floor((float)nrows / (float)numberOfThreads);
	int numOpThreadR = nrows % numberOfThreads;

	//printf("m = %d  r = %d\n", numOpThreadM, numOpThreadR);

	std::thread threadsList[numberOfThreads];
	
	for (int i = 0; i < numberOfThreads; ++i) {
		if (i + 1 == numberOfThreads && numOpThreadR > 0) {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, size, s));
			break;
		} else {
			threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1), size, s));
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
    int numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size*size];
    randmat(size, rand(), numThreads);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        printf("%f ", norm_points[i].x);
        printf("%f\n", norm_points[i].y);
      }
    }
  } else {


  }

  return 0;
}