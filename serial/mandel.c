#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const int MAX_STEPS = 150;
static int* matrix;

int calc (double x, double y) {
	int iter;
	double xx;
	for (iter = 0; iter < MAX_STEPS; ++iter)  {
		xx = x*x - y*y;
		y = 2.0 * x * y;
		x = xx;
		if ((x*x + y*y) >= 2.0) {
			return iter;
		}
	}
	return MAX_STEPS;
}

void mandel (const int size) {
	
	int i, j;	
	const double x0 = ((double)rand()/(double)RAND_MAX);
	const double y0 = ((double)rand()/(double)RAND_MAX);
	const double dx = ((double)rand()/(double)RAND_MAX)/ (double)(size - 1);
	const double dy = ((double)rand()/(double)RAND_MAX)/ (double)(size - 1);
	
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			matrix[i*size + j] = calc(x0 + ((double)i * dx), y0 + ((double)j * dy));
		}
	}
}

int main(int argc, char** argv) {

	if (argc == 3) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int print = atoi(argv[2]);

		matrix = (int*) malloc (sizeof(int) * size * size);

		mandel(size);

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