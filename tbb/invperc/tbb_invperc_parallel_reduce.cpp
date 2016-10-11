#include "tbb/tbb.h"
#include <iostream>
#include <cmath>

class FoundPoint {

public:

	int row, col, value;

	FoundPoint(): 
		row(-1), col(-1), value(INT_MAX) {};
	
	FoundPoint(int _row, int _col, int _value): 
		row(_row), col(_col), value(_value) {};

};

typedef tbb::blocked_range<std::size_t> range;
static int* matrix;
static int* mask;

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

bool setNewPoint(const int size, const FoundPoint& point) {
	
	if (point.row >= 0 && point.col >= 0) {	
		mask[point.row*size + point.col] = 1;
		return false;
	} else {
		return true;
	}
}

FoundPoint percolate (const int size) {
	 return 
	 tbb::parallel_reduce(
	 	range(1, size-1),
	 	FoundPoint(),
	 	[&](const range& r, FoundPoint point) -> FoundPoint {
             std::size_t end = r.end();
             for (std::size_t i = r.begin(); i != end; ++i) {
                for (int j = 1; j < size - 1; ++j) {
					if (mask[i*size + j]) {
						for (int sides = 0; sides < N_SIDES; ++sides) {
							int row = i + X_STEPS[sides];
							int col = j + Y_STEPS[sides];
							int pos = row*size + col;
							if (mask[pos] == 0 && matrix[pos] < point.value) {
								point.row = row;
								point.col = col;
								point.value = matrix[pos]; 
							}
						}
					}
				}
            }
            return point;
        },
        [](FoundPoint a, FoundPoint b) -> FoundPoint {

         	return a.value < b.value ? a : b;
         
        }
     );
}

void invperc (const int size, const int nfill) {
	
	int i;
	for (i = 0; i < nfill; ++i){
		FoundPoint point = percolate(size);
		if(setNewPoint(size, point)) { break; }
	}
}

void setMatrixValues (const int size) {

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix[i*size + j] = rand() % 1000;
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

		matrix = new int[size*size];
		mask = new int[size*size]();

		setThreadsNumber(numThreads);
		setMatrixValues(size);
		setMaskMiddlePoint(size);
		int nfill = 100000;

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