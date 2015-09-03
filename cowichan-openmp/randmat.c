#include "OpenMP.h"

void randmat (int nrows, int ncols, int s) { 

	const int VAL_A = 1313, VAL_B = 3131;
	int i = 0, j = 0, seed = 0;
	int *matrix = (int*)malloc((nrows + ncols) * sizeof(int));

    #pragma omp parallel for default(none) shared(matrix, seed)
  	for (i = 0; i < nrows; i++) {
    	
    	seed = s + i;
    	
    	for (j = 0; j < ncols; j++) {
    	 	
    	 	seed = VAL_A * seed + VAL_B;
    	  	matrix[i * ncols + j] = ((unsigned)seed) % 100;   
    	
    	}
  	}

  	for (j = 0; j < ncols + nrows; j++) {
    	 	
    	printf("%d", matrix[j]);    
    	
    }

}