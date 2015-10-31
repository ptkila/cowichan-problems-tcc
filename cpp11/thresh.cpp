#include "cpp11.h"

void reduceMax (int** matrix, int startIndex, int endIndex, int size, std::promise<int>* promise) {

  int i, j;
  int max = 0;

  for (i = startIndex; i < endIndex; i++) {
    for (j = 0; j < size; j++) {
      if (matrix[i][j] > max)
        max = matrix[i][j];
    }
  }
  promise->set_value(max);
}

void fillHistogram(int** matrix, int startIndex, int endIndex, int size, int* histogram) {
  for (int i = startIndex; i < endIndex; i++) {
    for (int j = 0; j < size; j++) {
      histogram[matrix[i][j]]++;
    }
  }
}

void fillMask(int** matrix, int startIndex, int endIndex, int size, int threshold, int** mask) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      mask[i][j] = (int)(matrix[i][j] > threshold);
    }
  }
}

void Cpp11::thresh(int** matrix, int size, int percent, int** mask) {
  int nMax = 0;
  int numberOfThreads = 4;
  int operationsByThread = size / 4;
  std::thread threadsList[numberOfThreads];
  std::promise<int> promises[numberOfThreads];
  std::future<int> futures[numberOfThreads];

  // Calc max
  for (int i = 0; i < numberOfThreads; ++i) {
    futures[i] = promises[i].get_future();
    threadsList[i] = std::thread(reduceMax, matrix, operationsByThread * i, operationsByThread * (i + 1), size, &promises[i]);
  }

  for (int i = 0; i < numberOfThreads; ++i) {
    int maxTmp = futures[i].get();
    if (nMax < maxTmp) {
      nMax = maxTmp;
    }
  }

  for (int i = 0; i < numberOfThreads; ++i) {
    threadsList[i].join();
  }

  // Fill histogram
  int* histogram = new int[nMax + 1];

  for (int i = 0; i < numberOfThreads; ++i) {
    threadsList[i] = std::thread(fillHistogram, matrix, operationsByThread * i, operationsByThread * (i + 1), size, histogram);
  }

  for (int i = 0; i < numberOfThreads; ++i) {
    threadsList[i].join();
  }

  int count = (size * size * percent) / 100;
  int prefixSum = 0;
  int threshold = nMax;

  for (int i = nMax; i >= 0 && prefixSum <= count; i--) {
    prefixSum += histogram[i];
    threshold = i;
  }

  // fill mask
  for (int i = 0; i < numberOfThreads; ++i) {
    threadsList[i] = std::thread(fillMask, matrix, operationsByThread * i, operationsByThread * (i + 1), size, threshold, mask);
  }

  for (int i = 0; i < numberOfThreads; ++i) {
    threadsList[i].join();
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      std::cout << mask[i][j] << " ";
    }
    std::cout << " ";
  }
}