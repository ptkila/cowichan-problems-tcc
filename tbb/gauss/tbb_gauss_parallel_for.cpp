#include "tbb/tbb.h"
#include <iostream>

typedef tbb::blocked_range<size_t> range;
static double* matrix;
static double* target;
static double* solution;

void elimination(const int size) {
    
    for (int i = 0; i < size - 1; ++i) {
    	tbb::parallel_for(
    		range(i + 1, size),
    		[&](const range& r) -> void {
    			std::size_t end = r.end();
	        	for (size_t j = r.begin(); j != end; ++j) {
	            	double mult = matrix[j*size + i]/ matrix[i*size + i];
            		
                    for (int k = i; k < size; ++k) {
                		matrix[j*size + k] -= matrix[i*size + k] * mult;
            		}
            		
            		target[j] -= target[i] * mult;
	        	}
    		}
    	);
	}
}

void fillSolution (const int size) {
    for (int i = size - 1; i >= 0; --i) {
        solution[i] = target[i];
        for (int j = size - 1; j > i; --j) {
            solution[i] -= matrix[i*size + j] * solution[j];
       	}
        solution[i] /= matrix[i*size + i];
    }
}

void gauss(const int size) {

    elimination(size);
    fillSolution(size);

}

void setTargetValues(const int size) {

    for (int i = 0; i < size; ++i) {
        target[i] = (double)(rand() % 1000);
    }
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
}

void setThreadsNumber(const int numThreads) {

	tbb::task_scheduler_init init(numThreads);

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

		setThreadsNumber(numThreads);
		setMatrixValues(size);
		setTargetValues(size);
		gauss(size);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				std::cout << solution[i] << " ";
			}
			std::cout << std::endl;
		}

		delete[] matrix;
		delete[] target;
		delete[] solution;

	} else {


	}

	return 0;
}