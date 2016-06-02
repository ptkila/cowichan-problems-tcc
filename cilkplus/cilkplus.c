	#include "cilkplus.h"
	#include <stdlib.h>
	#include <time.h>
	#include <limits.h>

	#define RANDMAT 1
	#define NORM 2
	#define OUTER 3
	#define PRODUCT 4
	#define THRESH 5

void set_threads_number (int t_num);

int main (int argc, char* argv[]) {

	// programa <problema> <num_threads> <printar>
	if (argc == 4) {
		int i, j;
		int matrix_size;
		int number_of_points;
		int percent;
		int** matrix_int;
		double** matrix_double;
		struct point *points;

		int num_problem = atoi(argv[1]);
		int num_threads = atoi(argv[2]);
		int print = atoi(argv[3]);

		// norm e outer
		number_of_points = 10;
		// produtc, randmat e thresh
		matrix_size = 10;
		// thresh
		percent = 50;

		set_threads_number(num_threads);
		srand (time(NULL));

		switch(num_problem){

			case (RANDMAT):

			randmat(matrix_size, matrix_size, rand(), num_threads, print);
			break;

			case (NORM):

			points = (struct point*) malloc(sizeof(struct point) * number_of_points);
			for (i = 0; i < number_of_points; i++) {
				points[i].x = (double)(i);
				points[i].y = (double)(i);
			}

			norm(points, number_of_points, num_threads, print);
			break;

			case(OUTER):

			points = (struct point*) malloc(sizeof(struct point) * number_of_points);

			for (i = 0; i < number_of_points; i++) {
				points[i].x = (double)rand() / (double)(RAND_MAX /INT_MAX);
				points[i].y = (double)rand() / (double)(RAND_MAX /INT_MAX);
			}

			outer(points, number_of_points, num_threads, print);
			break;

			case(PRODUCT):

			matrix_double = (double **)malloc(sizeof(double *)*matrix_size);
			for (i = 0; i < matrix_size; i++) {
				matrix_double[i] = (double *)malloc(matrix_size * sizeof(double));
			}

			double *vector = (double *)calloc (matrix_size, sizeof(double));

			for (i = 0 ; i < matrix_size; i++) {
				vector[i] = i;
				for (j = 0 ; j < matrix_size; j++) {
					matrix_double[i][j] = i + j;
				}
			}

			product(matrix_double, vector, matrix_size, num_threads, print);
			break;

			case(THRESH):

			matrix_int = (int **)malloc(matrix_size * sizeof(int *));
			for (i = 0; i < matrix_size; i++) {

				matrix_int[i] = (int *)malloc(matrix_size * sizeof(int));

			}

			int **mask = (int **)calloc(matrix_size, sizeof(int *));
			for (i = 0; i < matrix_size; i++) {
				mask[i] = (int *)calloc(matrix_size, sizeof(int));
			}

			for (i = 0 ; i < matrix_size; i++) {
				for (j = 0 ; j < matrix_size; j++) {
					matrix_int[i][j] = rand() % 255;
				}
			}

			thresh(matrix_int, matrix_size, percent, mask, num_threads, print);
			break;

			default:
			break;
		}
	} else {

		printf("programa <id_problema> <num de num_threads> <printar>\n");
	
	}

	return 0;
}

void set_threads_number (int t_num) {

  char threads[2];
  sprintf(threads,"%d", t_num);
  __cilkrts_end_cilk();  
  __cilkrts_set_param("nworkers", threads);

  //printf("%s\n",  threads );
  //printf("%d\n",  __cilkrts_get_nworkers() );
}