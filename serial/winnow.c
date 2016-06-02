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

int partition(struct weighted_point* vector, int l, int r) {
   int pivot, i, j, t;
   double x = 0;
   double y = 0;

   pivot = vector[l].weight;
   i = l;
   j = r + 1;
		
   for( ; ; ) {
   
   	do ++i; while( vector[i].weight <= pivot && i <= r );
   	do --j; while( vector[j].weight > pivot );
   	if( i >= j ) break;
   	
   	t = vector[i].weight; vector[i].weight = vector[j].weight; vector[j].weight = t;
   	x = vector[i].position.x; vector[i].position.x = vector[j].position.x; vector[j].position.x = x;
   	y = vector[i].position.y; vector[i].position.y = vector[j].position.y; vector[j].position.y = y;
   
   }
   
   t = vector[l].weight; vector[l].weight = vector[j].weight; vector[j].weight = t;
   x = vector[l].position.x; vector[l].position.x = vector[j].position.x; vector[j].position.x = x;
   y = vector[l].position.y; vector[l].position.y = vector[j].position.y; vector[j].position.y = y;

   return j;
   
}

void quick_sort(struct weighted_point* vector, int l, int r)
{
   int i = 0;

   if( l < r ) 
   {
       i = partition( vector, l, r);
       quick_sort( vector, l, i - 1);
       quick_sort( vector, i + 1, r);
   }
	
}

void fill_point_vector(struct weighted_point* vector, int vector_size, struct point* point_vector, int nelts) {

	int i = 0;
	int chunk = vector_size / nelts;
	for (i = 0; i < nelts; ++i) {
		int vector_index = i * chunk;
		point_vector[i] = vector[vector_index].position;
	}
}

struct point* winnow (int **matrix, int **mask, int size, int vector_size, int nelts) {

	int i, j;
	struct weighted_point* vector;
	struct point* point_vector;

	vector = (struct weighted_point*)malloc(sizeof(struct weighted_point) * vector_size);
	point_vector = (struct point*)malloc(sizeof(struct point) * nelts);

	fill_vector(matrix, mask, size, vector, vector_size);
	quick_sort(vector, 0, vector_size);
	fill_point_vector(vector, vector_size, point_vector, nelts);

	return point_vector;
}