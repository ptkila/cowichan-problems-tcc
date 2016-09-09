#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cmath>
#include <thread>
#include <mutex>

static int* matrix;
static int* mask;
static int* histogram;
static int numThreads;
static int nMax; 

void reduceMax (const int startIndex, const int endIndex, const int size) {
  int max = 0;

  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      if (matrix[i * size + j] > max)
        max = matrix[i*size + j];
    }
  }
  if(nMax < max) {
    nMax = max;
  }
}

void fillHistogram(const int startIndex, const int endIndex, const int size) {
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      histogram[matrix[i*size + j]]++;
    }
  }
}

void fillMask(const int startIndex, const int endIndex, const int size, const int threshold) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      mask[i*size + j] = (matrix[i*size + j] >= threshold);
    }
  }
}

void thresh(const int size, const int percent) {

  int nMax = 0;
  int numOpThreadM = (int)floor((float)size / (float)numThreads);
  int numOpThreadR = size % numThreads;  
  std::thread threadsList[numThreads];

  for (int i = 0; i < numThreads; ++i) {
    futures[i] = promises[i].get_future();
    if (i + 1 == numThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(reduceMax, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size);
      break;
    } else {
      threadsList[i] = std::thread(reduceMax, numOpThreadM * i, numOpThreadM * (i + 1), size);
    }
  }

  for (int i = 0; i < numThreads; ++i) {
    threadsList[i].join();
  }

  for (int i = 0; i < numThreads; ++i) {
    if (i + 1 == numThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(fillHistogram, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size);
      break;    
    } else {
      threadsList[i] = std::thread(fillHistogram, numOpThreadM * i, numOpThreadM * (i + 1), size);
    }
  }

  for (int i = 0; i < numThreads; ++i) {
    threadsList[i].join();
  }

  int count = (size * size * percent) / 100;
  int prefixSum = 0;
  int threshold = nMax;

  for (int i = nMax; i >= 0 && prefixSum <= count; i--) {
    prefixSum += histogram[i];
    threshold = i;
  }

  for (int i = 0; i < numThreads; ++i) {
    if (i + 1 == numThreads && numOpThreadR > 0) {
      threadsList[i] = std::thread(fillMask, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size);
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
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);
    int percent = 50;

    matrix = int[size * size];
    mask = int[size];
    histogram = int[256];
    nMax = std::numeric_limits<int>::lowest();

    setValuesMatrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          std:: cout << mask[i*size + j] << " ";
        }
        std::cout << std::endl;
      }
    }

    delete[] matrix;
    delete[] mask;
    delete[] histogram;

  } else {


  }

  return 0;
}