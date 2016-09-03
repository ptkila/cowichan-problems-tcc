#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const int MAX_STEPS = 150;
static int* matrix;
static int n_threads;

void fill_matrix (int const begin, int const end, int const size, 
	const double x0, const double y0, const double dx, const double dy) {

	if (begin + 1 == end) {
		int i, iter;
		double xx, x, y;
		int flag = 0;
		for (i = 0; i < size; ++i) {
			x = x0 + ((double)begin * dx);
			y = y0 + ((double)i * dy);
			for (iter = 0; iter < MAX_STEPS; iter++)  {
				xx = x*x - y*y;
				y = 2.0 * x * y;
				x = xx;
				if ((x*x + y*y) >= 2.0) {
					matrix[begin*size + i] = iter;
					flag = 1;
					break;
				}
			}
			if (!flag) {
				matrix[begin*size + i] = MAX_STEPS;
			} else {
				flag = 1;
			}
		}

		return;

	} else {

		int middle = begin + (end - begin)/ 2;
		cilk_spawn fill_matrix(begin, middle, size, x0, y0, dx, dy);
		cilk_spawn fill_matrix(middle, end, size, x0, y0, dx, dy);
		cilk_sync;

	}
}

void mandel (const int size) {
	
	const double x0 = ((double)rand()/(double)RAND_MAX);
	const double y0 = ((double)rand()/(double)RAND_MAX);
	const double dx = ((double)rand()/(double)RAND_MAX)/ (double)(size - 1);
	const double dy = ((double)rand()/(double)RAND_MAX)/ (double)(size - 1);
	
	cilk_spawn fill_matrix(0, size, size, x0, y0, dx, dy);
	cilk_sync;

}

void set_threads_number() {

	char threads[2];
	sprintf(threads,"%d", n_threads);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

}

int main(int argc, char** argv) {

	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		n_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		matrix = (int*) malloc (sizeof(int) * size * size);

		set_threads_number();
		mandel(size);

		if (print == 1) {
			int i, j;
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					printf("%d ", matrix[i*size + j]);
				}
				printf("\n");
			}
		}

		free(matrix);

	} else {


	}

	return 0;
}