#include "serial.h"

void fill_vector (int** matrix, int** mask, int size, struct weighted_point* vector, int vector_size) {
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (mask[i][j]) {
				struct weighted_point value;
				value.weight = matrix[i][j];
				value.position.x = i;
				value.position.y = j;
				vector[vector_size--] = value;
			}
		}
	}
}

void merge_sort(struct weighted_point* vector, int bot, int mid, int top, int vector_size) {

	int i, m, k, l;
	struct weighted_point temp[vector_size];

	l = bot;
	i = bot;
	m = mid + 1;

	while ((l <= mid) && (m <= top)) {

		if (vector[l].weight <= vector[m].weight) {
			temp[i] = vector[l];
			l++;
		}
		else {
			temp[i] = vector[m];
			m++;
		}
		i++;
	}

	if (l > mid) {
		for (k = m; k <= top; k++) {
			temp[i] = vector[k];
			i++;
		}
	}
	else {
		for (k = l; k <= mid; k++) {
			temp[i] = vector[k];
			i++;
		}
	}

	for (k = bot; k <= top; k++) {
		vector[k] = temp[k];
	}

}

void partition(struct weighted_point* vector, int bot, int top, int vector_size) {

	int mid;
	if (bot < top) {
		mid = (bot + top) / 2;
		partition(vector, bot, mid, vector_size);
		partition(vector, mid + 1, top, vector_size);
		merge_sort(vector, bot, mid, top, vector_size);
	}
}

void sort_vector (struct weighted_point* vector, int vector_size) {

	partition(vector, 0, vector_size - 1, vector_size);

}

void fill_point_vector(struct weighted_point* vector, int vector_size, struct point* point_vector, int nelts) {

	int i = 0;
	int chunk = vector_size / nelts;
	for (i = 0; i < nelts; ++i) {
		int vector_index = i * chunk;
		point_vector[i] = vector[vector_index].position;
	}
}

void winnow (int **matrix, int **mask, int size, int vector_size, int nelts) {

	int i, j;
	struct weighted_point* vector = (struct weighted_point*)malloc(sizeof(struct weighted_point) * vector_size);
	struct point* point_vector = (struct point*)malloc(sizeof(struct point) * nelts);

	fill_vector(matrix, mask, size, vector, vector_size);
	sort_vector(vector, vector_size);
	fill_point_vector(vector, vector_size, point_vector, nelts);

	for (i = 0; i < nelts; i++) {
		printf("%.0f ", point_vector[i].x);
		printf("%.0f\n", point_vector[i].y);
	}
}