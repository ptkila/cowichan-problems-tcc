#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <future>

static int* matrix;
static int* mask;
static int* histogram;
static int numThreads; 

void reduceMax (const int startIndex, const int endIndex, const int size, 
  std::promise<int>* promise) {
  int max = 0;

  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      if (matrix[i * size + j] > max)
        max = matrix[i*size + j];
    }
  }
  promise->set_value(max);
}

template<class F>
int setTasksFuture(F&& f, const int numOpThreadM, const int numOpThreadR, const int size) {
  
  std::vector <std::thread> threads;
  std::promise<int> promises[numThreads];
  std::future<int> futures[numThreads];
  int max = -1;

  for (int i = 0; i < numThreads; ++i) {
    futures[i] = promises[i].get_future();
    if (i + 1 == numThreads && numOpThreadR > 0 || numOpThreadM == 0) {
      threads.push_back(std::thread(f, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size, &promises[i]));
      break;
    } else {
      threads.push_back(std::thread(f, numOpThreadM * i, 
        numOpThreadM * (i + 1), size, &promises[i]));
    }
  }

  for (auto &fut : futures){
    int v = fut.get();
    if (max < v){
      max = v;
    }
  }

  for (auto &t : threads) {
    t.join();
  }

  return max;
}

void fillHistogram(const int startIndex, const int endIndex, const int size) {
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      ++histogram[matrix[i*size + j]];
    }
  }
}

void fillMask(const int startIndex, const int endIndex, const int size,
 const int threshold) {
  for (int i = startIndex; i < endIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      mask[i*size + j] = (matrix[i*size + j] >= threshold);
    }
  }
}

int calcThreshold (const int percent, const int nMax, const int size) {
  int i;
  int count = (size * size * percent)/ 100;
  int prefixsum = 0;
  int threshold = nMax;

  for (i = nMax; i >= 0 && prefixsum <= count; --i) {
    prefixsum += histogram[i];
    threshold = i;
  }

  return threshold;
}

void thresh(const int size, const int percent) {

  int numOpThreadM = (int)floor((float)size / (float)numThreads);
  int numOpThreadR = size % numThreads;  
  std::vector <std::thread> threads;

  int nMax = setTasksFuture(reduceMax, numOpThreadM, numOpThreadR, size);
  
  for (int i = 0; i < numThreads; ++i) {
    if (i + 1 == numThreads && numOpThreadR > 0 || numOpThreadM == 0) {
      threads.push_back(std::thread(fillHistogram, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size));
      break;
    } else {
      threads.push_back(std::thread(fillHistogram, numOpThreadM * i, 
        numOpThreadM * (i + 1), size));
    }
  }

  for (auto &t : threads) {
    t.join();
  }

  threads.clear();

  int threshold = calcThreshold(percent, nMax, size);

  for (int i = 0; i < numThreads; ++i) {
    if (i + 1 == numThreads && numOpThreadR > 0 || numOpThreadM == 0) {
      threads.push_back(std::thread(fillMask, numOpThreadM * i, 
        numOpThreadM * (i + 1) + numOpThreadR, size, threshold));
      break;
    } else {
      threads.push_back(std::thread(fillMask, numOpThreadM * i, 
        numOpThreadM * (i + 1), size, threshold));
    }
  }

  for (auto &t : threads) {
    t.join();
  }
  
}

void setValuesMatrix(const int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i*size + j] = rand() % 256;    
    }
  }
  /*
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      std::cout << matrix[i*size + j] << " ";    
    }
    std::cout << "\n";
  }
  */
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size * size];
    mask = new int[size];
    histogram = new int[256];
    int percent = 50;

    setValuesMatrix(size);
    thresh(size, percent);

    if (print == 1) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
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