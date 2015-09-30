#include "openmp.h"

void randmat (int nrows, int ncols, int s) {

    const int VAL_A = 1313, VAL_B = 3131;
    int i = 0, seed = 0;
    int matrixTotalSize = ncols * nrows;
    int *matrix = (int *)malloc((matrixTotalSize) * sizeof(int));

    #pragma omp parallel for default(none) shared(matrix, seed, matrixTotalSize, s)
    for (i = 0; i < matrixTotalSize; ++i) {

        seed = s + i;
        seed = VAL_A * seed + VAL_B;
        matrix[i] = seed;

    }

    for (i = 0; i < matrixTotalSize; i++) {

        printf("%d ", matrix[i]);

    }

}