#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>

static double* matrix;
static double* vector;
static double* result;
static int numThreads;

void fillValues(const int startIndex, const int lastIndex, const int size) {
	for (int i = startIndex; i < lastIndex; ++i) {
		double sum = 0.0;
		for (int j = 0; j < size; ++j) {
			sum += matrix[i*size + j] * vector[j];
		}
		result[i] = sum;
	}
}

template<class F>
void setTasks(F&& f, const int numOpThreadM, const int numOpThreadR, const int size) {
	
	std::vector <std::thread> threads;

	for (int i = 0; i < numThreads; ++i) {
		if (i + 1 == numThreads && numOpThreadR > 0) {
			threads.push_back(std::thread(f, numOpThreadM * i, 
				numOpThreadM * (i + 1) + numOpThreadR, size));
			break;
		} else {
			threads.push_back(std::thread(f, numOpThreadM * i, 
				numOpThreadM * (i + 1), size));
		}
	}

	for (auto &t : threads) {
		t.join();
	}
}

/*
template<class F>
void setTasks(F&& f, const int numOpThreadM, const int numOpThreadR, const int size, 
	ThreadPool& pool) {
	
	for (int i = 0; i < numThreads; ++i) {
		if (i + 1 == numThreads && numOpThreadR > 0 || numOpThreadM == 0) {
			pool.enqueue(f, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, size);
			break;
		} else {
			pool.enqueue(f, numOpThreadM * i, numOpThreadM * (i + 1), size);
		}
	}

	pool.waitForAll();
} 
*/

void product(const int size) {

	//ThreadPool pool(numThreads);
	int numOpThreadM = (int)floor((float)size / (float)numThreads);
	int numOpThreadR = size % numThreads;

	setTasks(fillValues, numOpThreadM, numOpThreadR, size);

}

void setValuesMatrix(const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 10;    
		}
	}
	/*
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << matrix[i*size + j] << " ";    
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
}

void setValuesVector(const int size) {
	for (int i = 0; i < size; ++i) {
		vector[i] = rand() % 10;
	}
	/*
	for (int i = 0; i < size; ++i) {
		std::cout << vector[i] << " ";
	}
	std::cout << std::endl;
	*/
}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new double[size * size];
		result = new double[size];
		vector = new double[size];

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