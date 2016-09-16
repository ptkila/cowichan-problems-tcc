#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct point_w {

	int weight, i, j;

};

static int* matrix;
static int* mask;
static struct point_w* ev_values;
static struct point_w* points;
static int nelts;

int compare (const void * a, const void * b) {

  const struct point_w* point0 = (struct point_w*) a;
  const struct point_w* point1 = (struct point_w*) b;
  return (point1->weight - point0->weight);

}

void fill_values(const int size) {
  
  int i, j;
  int count = 0;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      if (mask[i*size + j] == 1) {
        ev_values[count].weight = matrix[i*size + j];
        ev_values[count].i = i;
        ev_values[count].j = j;
        count++;
      }
    }
  }
}

int count_points(const int size) {

	int i, j;
	int count = 0;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (mask[i*size + j] == 1) {
				count++;
			}
		}
	}
  return count;
}

void set_values_matrix(const int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      matrix[i*size +j] = rand();
    }
  }
}

void set_values_mask(const int size) {
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      mask[i*size +j] = rand() % 2;
    }
  }
}

void fill_points(const int len) {
  int i;
  int chunk = len/ nelts;
  for(i = 0; i < nelts; ++i) {
    points[i] = ev_values[i*chunk];
  }
}

void winnow (const int size) {

  int len = count_points(size);

  ev_values = (struct point_w*) malloc (sizeof(struct point_w) * len);

  fill_values(size);

  qsort(ev_values, len, sizeof(*ev_values), compare);
  
  // Garante nelts <= n_points && > 0
  nelts = rand() % len;
  if (!nelts) {
    nelts = 1;
  }
  
  points = (struct point_w*) malloc (sizeof(struct point_w) * nelts);
  fill_points(len);

}

int main(int argc, char *argv[]) {
  
  if (argc == 3) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int print = atoi(argv[2]);

    matrix = (int*) malloc (sizeof(int) * size * size);
    mask = (int*) malloc (sizeof(int) * size * size);

    set_values_matrix(size);
    set_values_mask(size);

    winnow(size);

    if (print) {
      int i;
      for (i = 0; i < nelts; ++i) {
        printf("%d %d %d\n", points[i].i, points[i].j, points[i].weight);
      }
    }

    free (matrix);
    free (mask);
    free (ev_values);
    free (points);

  } else {

    printf("programa <tamanho> <printar>\n");

  }

  return 0;
}