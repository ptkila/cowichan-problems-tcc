#include "tbb.h"

void doTheMath (double **matrix, double *vector, int size, double** result) {

	tbb::parallel_for(tbb::blocked_range2d<int, int> (0, size, 0, 0, size, 0), [ = ] (tbb::blocked_range2d<int, int> r)
	{
		for (int i = r.rows().begin(); i < r.rows().end(); i++) {
			for (int j = r.cols().begin(); j < r.cols().end(); j++) {
				result[i][j] += matrix[i][j] * vector[j];
			}
		}
	});
}

void TBB::product (double **matrix, double *vector, int size) {

	double** result = new double*[size];
	for (int i = 0; i < size; ++i) {
		result[i] = new double[size];
	}

	doTheMath(matrix, vector, size, result);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << result[i][j] << " ";
		}
		std::cout << "\n";
	}
}