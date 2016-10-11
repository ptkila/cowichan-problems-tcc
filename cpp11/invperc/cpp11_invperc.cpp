#include <iostream>
#include <climits>
#include "../ThreadPool.h"

class FoundPoint {

public:

	int row, col, value;

	FoundPoint(): 
		row(-1), col(-1), value(INT_MAX) {};
	
	FoundPoint(int _row, int _col, int _value): 
		row(_row), col(_col), value(_value) {};

};

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

bool setNewPoint(const int size, const FoundPoint point) {
	
	if (point.row >= 0 && point.col >= 0) {	
		mask[point.row*size + point.col] = 1;
		return false;
	} else {
		return true;
	}
}

FoundPoint calc (const int startIndex, const int lastIndex, const int size) {

	FoundPoint found = FoundPoint();
	for (int i = startIndex; i < lastIndex; ++i) {
		for (int j = 1; j < size - 1; ++j) {
			if (mask[i*size + j] == 1 && (i > 0 && i < size - 1)) {
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

	return found;

}

FoundPoint percolate (const int size, ThreadPool& pool) {

	std::vector<std::future<FoundPoint>> points;
	FoundPoint max = FoundPoint();
	int numThreads = pool.getSize();
	int numOpThreadM = size/ numThreads;
	int numOpThreadR = size % numThreads;
	bool end = false;  

	for (int i = 0; i < numThreads; ++i) {
            
            int startIndex = numOpThreadM * i;
            int lastIndex = numOpThreadM * (i + 1);

            if ((i + 1 == numThreads && numOpThreadR > 0) 
            	|| numOpThreadM == 0) {
             	lastIndex += numOpThreadR;
                end = true;
            }    

            points.emplace_back(pool.enqueue_return(calc, startIndex, 
            	lastIndex, size));
            
            if (end) break;
    }

    for (auto& f: points) {
    	FoundPoint tmpFP = f.get();
    	if (tmpFP.value < max.value){
    		max = tmpFP;
    	}
    }

	return max;
}

void invperc (const int size, const int nfill, const int numThreads) {
	
	ThreadPool pool(numThreads);

	int i;
	for (i = 0; i < nfill; ++i){
		
		FoundPoint point = percolate(size, pool);
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

int main (int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int numThreads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = new int[size*size];
		mask = new int[size*size]();
		int nfill = 100000;

		setMatrixValues(size);
		setMaskMiddlePoint(size);
		invperc(size, nfill, numThreads);

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