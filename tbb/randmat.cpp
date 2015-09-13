#include "tbb.h" 

void TBB::randmat (int nrows, int ncols, int s) {

	const int VAL_A = 1313, VAL_B = 3131;
    int matrixTotalSize = ncols * nrows;   
	int *matrix = new int[matrixTotalSize];

    tbb::parallel_for(tbb::blocked_range<int>(0, matrixTotalSize), [=](tbb::blocked_range<int> r) {
      auto end = r.end (); 
      for (int i = r.begin(); i != end; ++i) {
        int seed = s + i;
        seed = VAL_A * seed + VAL_B;
    	matrix[i] = seed % 100; 
      }
  	});

  	for (int i = 0; i < matrixTotalSize; i++) {
    	 	
    	printf("%d ", matrix[i]);    
    	
    }
}