#include "cpp11.h"

static int* matrix;
static int* mask;
static int* histogram; 

void reduceMax (const int startIndex, const int endIndex, const int size, std::promise<int>* promise) {
  int max = 0;

  for (int i = startIndex; i < endIndex; i++) {
    for (int j = 0; j < size; j++) {
      if (matrix[i * size + j] > max)
        max = matrix[i*size + j];
    }
  }
  promise->set_value(max);
}

void fillHistogram(const int startIndex, const int endIndex, const int size) {
  for (int i = startIndex; i < endIndex; i++) {
    for (int j = 0; j < size; j++) {
      histogram[matrix[i*size + j]]++;
    }
  }
}

void fillMask(const int startIndex, const int endIndex, const int size, const int threshold) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      mask[i*size + j] = (matrix[i*size + j] >= threshold);
    }
  }
}

void thresh(const int size, const int percent, const int numberOfThreads) {

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
      threadsList[i] = std::thread(reduceMax, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, size, &promises[i]);
      break;
    } else {
      threadsList[i] = std::thread(reduceMax, numOpThreadM * i, numOpThreadM * (i + 1), size, &promises[i]);
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

  for (int i = 0; i < numberOfThreads; ++i) {
    if (i + 1 == numberOfThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(fillHistogram, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, size);
      break;    
    } else {
      threadsList[i] = std::thread(fillHistogram, numOpThreadM * i, numOpThreadM * (i + 1), size);
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
      threadsList[i] = std::thread(fillMask, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, size);
      break;
    } else {
      threadsList[i] = std::thread(fillMask, numOpThreadM * i, numOpThreadM * (i + 1), size);
    }
  }

  for ( auto &t : threadsList ) {
    t.join();
  }
}

void setValuesMatrix(const int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 255;    
    }
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = int[size * size];
    mask = int[size];
    histogram = int[256];

    setValuesMatrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          printf("%hhu ", mask[i*size + j]);
        }
        printf("\n");
      }
    }
  } else {


  }

  return 0;
}