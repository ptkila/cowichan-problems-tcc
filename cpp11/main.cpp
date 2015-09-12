#include <iostream>
#include <thread>
#include <stdio.h>

const int size = 16;
const int num_threads = 4;
const int parts = size/num_threads;

double matrix[size][size];
double candidate[size];
double solution[size];

void product (int start_index, int end_index) {

	for (int i = start_index; i < end_index; ++i) {
        for (int j = start_index; j < end_index; ++j) {
                solution[i] += matrix[i][j] * candidate[j];            
        }
    }
} 

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

    std::thread t[num_threads];
 
    for (int i = 0; i < num_threads; ++i) {
 
        t[i] = std::thread(product, i * parts, i * parts + 1);
 
    }
  
    for (int i = 0; i < num_threads; ++i) {
    
        t[i].join();
    
    }

    for(int i = 0; i < size; i++) {
            printf("%0.f ", solution[i]);
            printf("\n");
    } 

    return 0;

}