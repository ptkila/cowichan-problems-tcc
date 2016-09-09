#include "tbb/tbb.h"
#include "tbb/blocked_range2d.h"
#include <iostream>

class foundPoint {
public:

	int row, col, value;

	foundPoint() {

		this->row = 0;
		this->col = 0;
		this->value = 0;
	}
	
	void reset() {

		this->row = -1;
		this->col = -1;
		this->value = INT_MAX;
	
	}
};

typedef tbb::blocked_range2d<size_t, size_t> range2d;
static int* matrix;
static int* mask;
static int nfill;
static int numThreads;
static foundPoint found;

static const int N_SIDES = 4;
static const int X_STEPS[4] = {1, 0, -1, 0};
static const int Y_STEPS[4] = {0, 1, 0, -1};

void setMaskMiddlePoint (const int size) {
	if (size % 2 == 0) {
		mask[(size*size)/2 + size/2] = 1;
	} else {
		mask[(size*size)/2] = 1;
	}
}

int setNewPoint(const int size) {
	if (found.row >= 0 && found.col >= 0) {	
		mask[found.row*size + found.col] = 1;
		return 0;
	} else {
		return 1;
	}
}

bool evaluateNeighbors (const int row, const int col, const int size) {

	return !((row < 0 || row >= size) || (col < 0 || col >= size)) ? true : false; 

}

void percolate (const int size) {
	tbb::parallel_for(range2d(0, size-1, 0, size-1),[&](const range2d& r) {
		size_t r_end = r.rows().end();
		for (size_t i = r.rows().begin(); i != r_end; ++i) {
			size_t c_end = r.cols().end();
			for (size_t j = r.cols().begin(); j != c_end; ++j) {
				if (mask[i*size + j]) {
					for (int sides = 0; sides < N_SIDES; ++sides) {
						int row = i + X_STEPS[sides];
						int col = j + Y_STEPS[sides];
						int pos = row*size + col;
						if (mask[pos] == 0 && matrix[pos] < found.value) {
							found.row = row;
							found.col = col;
							found.value = matrix[pos]; 
						}
					}
				}
			}
		}
	});
}

void invperc (const int size, const int nfill) {
	
	int i;
	int j, k;
	for (i = 0; i < nfill; ++i){
		found.reset();
		percolate(size);
		if(setNewPoint(size))
			break;
		/*
		for (k = 0; k < size; k++) {
			for (j = 0; j < size; j++) {
				printf("%d ", mask[k*size + j]);
			}
			printf("\n");
		}
		printf("\n");
		*/
	}
}

void setThreadsNumber() {

	tbb::task_scheduler_init init(numThreads);

}

void setMatrixValues (const int size) {

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 1000;
		}
	}
	
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << matrix[i*size + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main (int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size*size];
		mask = new int[size*size]();
		found = foundPoint();
		nfill = 1000;

		setThreadsNumber();
		setMatrixValues(size);
		setMaskMiddlePoint(size);
		invperc(size, nfill);

		if (print == 1) {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					std::cout << mask[i*size + j] << " ";
				}
				std::cout << std::endl;
			}
		}

		delete[] matrix;
		delete[] mask;

	} else {


	}

	return 0;
}