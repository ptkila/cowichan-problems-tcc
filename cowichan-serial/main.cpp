#include <stdio.h>

const int size = 2;

double matrix[size][size];
double candidate[size];
double solution[size];

int main()
{
    // Initialize buffers.
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = (double)i + j;
        }
        candidate[i] = (double)i;
        solution[i] = 0.0f;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
                solution[i] += matrix[i][j] * candidate[j];
        }
    }

    for(int i = 0; i < size; i++) {
            printf("%2.f ", solution[i]);
            printf("\n");
    } 

    return 0;
}