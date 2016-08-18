#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct point {
  double x;
  double y;
};

static double *matrix;
static double *vector;
static struct point* points;

double sqr(const double x) {
	return x * x;
}

double distance(const struct point a, const struct point b) {
	return sqrt(sqr(a.i - b.i) + sqr(a.j - b.j));
}

void outer(const int size) {
	double nmax = 0;
	int i, j;
	struct point origin;
	
	cilk_for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (i != j) {
				matrix [i*size + j] = distance(points[i], points[j]);
				nmax = fmax(nmax, matrix [i*size + j]);
			}      
		}
		matrix [i*size + i] = nmax * size;
		vector [i] = distance(origin, points[i]);
	}
}


void set_vector_of_points(const int size) {
	int i, a, b;
	for (i =  0; i < size; i++) {
		points[i].i = rand();
		points[i].j = rand();
	}
}

void set_threads_number (const int t_num) {

	char threads[2];
	sprintf(threads,"%d", t_num);
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}

int main(int argc, char** argv) {
	
	if (argc == 4) {

		srand (time(NULL));
		int size = atoi(argv[1]);
		int num_threads = atoi(argv[2]);
		int print = atoi(argv[3]);
		int i, j;

		matrix = (double*) malloc (sizeof(double) * size * size);
		vector = (double*) malloc (sizeof(double) * size);
		points = (struct point*) malloc (sizeof(Point) * size);

		set_vector_of_points(size);
		set_threads_number(num_threads);
		outer(size);

		if (print == 1) {
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					printf("%g ", matrix[i*size + j]);
				}
				printf("\n");
			}
			printf("\n");

			for (i = 0; i < size; i++) {
				printf("%g ", vector[i]);
			}
			printf("\n");
		}

	} else {

		printf("programa <tamanho> <num de num_threads> <printar>\n");

	}

	return 0;
}
