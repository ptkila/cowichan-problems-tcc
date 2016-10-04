#include <iostream>
#include "../ThreadPool.h"

static double* matrix;
static double* target;
static double* solution;

void elimination(const int startIndex, const int lastIndex, const int size,
	const int iter) {

    //i = diagonal principal
    //j = linha abaixo de i
    //k = colunas de j
    
	for (int j = startIndex + iter + 1; j < lastIndex; ++j) {
		double mult = matrix[j*size + iter]/ matrix[iter*size + iter];		
		//Atualiza linha
		for (int k = iter; k < size; ++k) {
    		matrix[j*size + k] -= matrix[iter*size + k] * mult;
		}
		// Atualiza vetor
		target[j] -= target[iter] * mult;
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

void gauss(const int size, const int numThreads) {

	ThreadPool pool(numThreads);

	for (int i = 0; i < size - 1; ++i) {
		pool.parallel_for(elimination, size, i);
		pool.waitAll();
	}

    fillSolution(size);

}

void setTargetValues(const int size) {

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
            printf("%f ", matrix[i*size + j]);
        }
        printf("\n");
    }
    printf("\n");
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

		/*
		// Testar valores
		double* result = new double[size]();
		for (int i = 0; i < size; ++i) {
     		for (int j = 0; j < size; ++j) {
        		result[i] += matrix[i*size + j] * solution[j];
      		}
    	}

		for (int i = 0; i < size; ++i) {
        	std::cout << result[i] << " = " << target[i] << std::endl;
    	}
		*/

		delete[] matrix;
		delete[] target;
		delete[] solution;

	} else {


	}

	return 0;
}