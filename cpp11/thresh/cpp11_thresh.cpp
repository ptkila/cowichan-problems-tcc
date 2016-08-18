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

int** Cpp11::thresh(int** matrix, int size, int percent, int** mask, int numberOfThreads, int shouldPrint) {

  int nMax = 0;
  int numOpThreadM = (int)floor((float)size / (float)numberOfThreads);
  int numOpThreadR = size % numberOfThreads;  
  std::thread threadsList[numberOfThreads];
  std::promise<int> promises[numberOfThreads];
  std::future<int> futures[numberOfThreads];

  // Calc max
  for (int i = 0; i < numberOfThreads; ++i) {
    futures[i] = promises[i].get_future();
    if (i + 1 == numberOfThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(reduceMax, matrix, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadM + numOpThreadR, size, &promises[i]);
      break;
    } else {
      threadsList[i] = std::thread(reduceMax, matrix, numOpThreadM * i, numOpThreadM * (i + 1), size, &promises[i]);
    }
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
    if (i + 1 == numberOfThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(fillHistogram, matrix, numOpThreadM * i, numOpThreadM * (i + 1) 
        + numOpThreadM + numOpThreadR, size, histogram);
      break;    
    } else {
      threadsList[i] = std::thread(fillHistogram, matrix, numOpThreadM * i, numOpThreadM * (i + 1), size, 
        histogram);
    }
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
    if (i + 1 == numberOfThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(fillMask, matrix, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadM + numOpThreadR, size, threshold, mask);
      break;

    } else {
      threadsList[i] = std::thread(fillMask, matrix, numOpThreadM * i, numOpThreadM * (i + 1), size, 
        threshold, mask);
    }
  }

  for ( auto &t : threadsList ) {
    t.join();
  }

  if (shouldPrint == 1){
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        std::cout << mask[i][j] << " ";
      }
      std::cout << "\n";
    }
  }

  return mask;

}