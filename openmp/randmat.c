#include "openmp.h"

int** randmat (int nrows, int ncols, int s, int num_threads, int should_print) {

    const int VAL_A = 1313, VAL_B = 3131;
    int i = 0, j = 0, seed = 0;
    int **matrix;

    matrix = (int**)malloc(nrows * sizeof(int*));
    for (i = 0; i < nrows; ++i) {
        matrix[i] = (int*)malloc(ncols * sizeof(int));
    }

    #pragma omp parallel shared(matrix, seed, nrows, ncols, s)
    {
        #pragma omp for
        for (i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                seed = s + i + j;
                seed = VAL_A * seed + VAL_B;
                matrix[i][j] = seed % 100;
            }
        }
    }


    if (should_print == 1) {
        for (i = 0; i < nrows; i++) {
            for (j = 0; j < ncols; j++) {
                printf("%d ", matrix[i][j] );
            }
            printf("\n");
        }
    }

    return matrix;

}