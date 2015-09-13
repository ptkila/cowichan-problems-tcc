#include "serial.h"

void randmat (int nrows, int ncols, int s) { 

	const int VAL_A = 1313, VAL_B = 3131;
	int i = 0, seed = 0;
    int matrixTotalSize = nrows * ncols;
	int *matrix = (int*)malloc((matrixTotalSize) * sizeof(int));

  	for (i = 0; i < matrixTotalSize; i++) {
    	
    	seed = s + i; 	
    	seed = VAL_A * seed + VAL_B;
    	matrix[i] = seed % 100;   
    	
  	}

  	for (j = 0; j < ncols + nrows; j++) {
    	 	
    	printf("%d", matrix[j]);    
    	
    }

}