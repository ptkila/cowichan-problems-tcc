#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const int VAL_A = 1313, VAL_B = 3131;
static int *matrix;

void randmat(const int size, const int seed) {
  int i, j, s;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      s = VAL_A * (seed + i + j) + VAL_B;
      matrix[i*size + j] = s % 100;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);

    matrix = (int*) malloc (sizeof(int) * size * size);

    randmat(size, rand());

    if (print == 1) {
      int i, j;
      for (i = 0; i < size; ++i) {
          for (j = 0; j < size; ++j) {
              printf("%d ", matrix[i*size + j]);
          }
         printf("\n");
        }
    } 

    free(matrix);

  } else {

    printf("programa <tamanho> <printar>\n");

  }

  return 0;

}