#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <mutex>
#include <vector>
#include "../ThreadPool.h"

static double* matrix;
static double* target;
static double* solution;
static std::mutex mutex;

void fillSolution (const int size) {
	/**/
	// back-tracking
	for (int i = size - 1; i >= 0; --i) {
    	// x0, x1, ...
    	solution[i] = target[i];
    	// Substitui a var x0, x1, ... e reduz do final
    	for (int j = size - 1; j > i; --j) {
      		solution[i] -= matrix[i*size + j] * solution[j];
    	}
    	// Divide pela diagonal princ?
    	solution[i] /= matrix[i*size + i];
  	}
}

int getPivot(const int j, const int size) {

	int k = j;
    double y_best, y_cmp;
    y_best = abs(matrix[k*size + j]);

    for(int i = j + 1; i < size; ++i) {
        y_cmp = abs(matrix[i*size + j]);
        if(y_cmp > y_best) {
            k = i;
            y_best = y_cmp;
        }
    }
    return k;
}

void swapLineMatrix(const int p, const int ksave, const int size) {
	double tmp = 0.0;
	int iStart = p*size;
	int iEnd = p*size + size;
	int jStart = ksave*size;
	int jEnd = ksave*size + size;
	
	for (int i = iStart; i < iEnd; ++i){
		if (jStart < jEnd) {
			tmp = matrix[i];
			matrix[i] = matrix[jStart];
			matrix[jStart] = tmp;
			jStart++;
		}
	}
} 

void swapValueTarget(const int p, const int ksave) {
	double tmp = target[p];
	target[p] = target[ksave];
	target[ksave] = tmp;
}

void elimination(const int j, const int i, const int p, const int size) {

	double m = 0.0;
	mutex.lock();
	for (int k = i + j + 1; k < size; k = k + p) {
		m = matrix[k*size + j]/ matrix[j*size + j];
		for (int l = j; l < size; ++l) {
			matrix[k*size + l] -= m * matrix[j*size + l];
		}
		target[k] -= m * target[j];
	}
	mutex.unlock();
}

void gauss(const int size, const int numThreads) {

	ThreadPool pool(numThreads);
	int ksave;
    
    for(int j = 0; j < size - 1; ++j) {
    	
    	ksave = getPivot(j, size);
    	swapLineMatrix(j, ksave, size);
    	swapValueTarget(j, ksave);
    	
    	for (int i = 0; i < numThreads; ++i) {

    		pool.enqueue(elimination, j, i, numThreads, size);
    	
    	}
    }

	// Testar eliminação
	/*
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << matrix[i*size + j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
    fillSolution(size);

}

void setTargetValues (const int size) {
	for (int i = 0; i < size; ++i) {
		target[i] = (double)(rand() % 1000);
	}
	
	/*
	target[0] = 2;
	target[1] = 4;
	target[2] = 0;
	*/
}	

void setMatrixValues (const int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (i == j) {
				matrix[i*size + j] = (double)(rand() % 1000);
			} else {
				matrix[i*size + j] = (double)(rand() % 100);
			}
		}
	}

	/*
	matrix[0] = 1;
	matrix[1] = 1;
	matrix[2] = 0;

	matrix[3] = 2;
	matrix[4] = -1;
	matrix[5] = 3;

	matrix[6] = -1;
	matrix[7] = 0;
	matrix[8] = 1;
	*/
	/*
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << matrix[i*size + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
}

int main (int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new double[size*size];
		target = new double[size];
		solution = new double[size]();

		setMatrixValues(size);
		setTargetValues(size);
		gauss(size, numThreads);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				std::cout << solution[i] << " ";
			}
			std::cout << std::endl;
		}

		// Testar valores
		double* result = new double[size]();
		for (int i = 0; i < size; ++i) {
     		for (int j = 0; j < size; ++j) {
        		result[i] += matrix[i*size + j] * solution[j];
      		}
    	}
    	int count = 0;		
		for (int i = 0; i < size; ++i) {
        	std::cout << result[i] << " = " << target[i] << "\n"; 
    	}
		
		delete[] matrix;
		delete[] target;
		delete[] solution;

	} else {


	}

	return 0;
}