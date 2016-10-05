#include <algorithm>
#include <iostream>
#include "../ThreadPool.h"

class PointW {

public:

  int i, j, weight;

  PointW(): 
    i(0), j(0), weight(0) {};

  PointW(int _i, int _j, int _weight):
    i(_i), j(_j), weight(_weight) {};

};

static int* matrix;
static int* mask;
static int* offsets;
static PointW* evValues;
static PointW* points;

void fillValues(const int startIndex, const int lastIndex, const int size,
  const int n_threads) {
  int index = startIndex/ (size/ n_threads);
  int count = offsets[index];
  for (int i = startIndex; i < lastIndex; ++i) {
    for (int j = 0; j < size; ++j) {
      if (mask[i*size + j] == 1) {
        int v = matrix[i*size + j];
        evValues[count] = PointW(i, j, v);
        ++count;
      }
    }
  }
}

int calcPoints(const int startIndex, const int lastIndex, const int size) {
  int count = 0;
  for (int i = startIndex; i < lastIndex; ++i) {
    for (int j = 0; j < size; ++j) {
       if (mask[i*size + j] == 1) {
         count++;
       }
    }
  }
  return count;
}

int countPoints (ThreadPool& pool, const int size) {

  std::vector<std::future<int>> count_per_lines;
  int numThreads = pool.getSize();
  int numOpThreadM = size / numThreads;
  int numOpThreadR = size % numThreads;
  bool end = false;  

  for (int i = 0; i < numThreads; ++i) {
    
    int startIndex = numOpThreadM * i;
    int lastIndex = numOpThreadM * (i + 1);

    if ((i + 1 == numThreads && numOpThreadR > 0) || numOpThreadM == 0) {
      lastIndex += numOpThreadR;
      end = true;
    }

    count_per_lines.emplace_back(pool.enqueue_return(calcPoints, startIndex, 
      lastIndex, size));        
  
    if (end) break;
  
  }

  std::vector<int> values;
  for (auto& v: count_per_lines) {
    values.emplace_back(v.get());
  }

  for (int i = 0; i < values.size(); ++i) {
    offsets[i] = values[i];
  }
  /*
  for (int i = 0; i <= numThreads; ++i)
  {
    std::cout << offsets[i] << " ";
  }
  std::cout << std::endl;
  */
  int len = 0;
  for (int i = 0; i <= values.size(); ++i) {
    int tmp = offsets[i];
    offsets[i] = len;
    len += tmp;
  }
  /*
  for (int i = 0; i <= numThreads; ++i)
  {
    std::cout << offsets[i] << " ";
  }
  std::cout << std::endl;
  */
  return len;
}

void fillPoints(const int startIndex, const int lastIndex, const int nelts,
  const int len) {
  const int chunk = len/ nelts;

  //std::cout << len << " " << nelts << " " << chunk << std::endl;

  for (int i = startIndex; i < lastIndex; ++i) {
    points[i] = evValues[i * chunk];
  }
}

void winnow(const int size, const int nelts, const int numThreads) {

  ThreadPool pool(numThreads);

  int len = countPoints(pool, size);
  evValues = new PointW[len];

  //std::cout << len << std::endl;

  pool.parallel_for(fillValues, size, numThreads);
  pool.waitAll();

  /*
  for (int i = 0; i < len; ++i)
  {
    std::cout << evValues[i].i << " " << 
      evValues[i].j << " " << evValues[i].weight << std::endl;
  }
  std::cout << std::endl;
  */

  std::sort(evValues, evValues + len, 
    [&](const PointW& a, const PointW& b) -> bool { 
      return a.weight < b.weight; 
    }
  );

  /*
  for (int i = 0; i < len; ++i)
  {
    std::cout << evValues[i].i << " " << 
      evValues[i].j << " " << evValues[i].weight << std::endl;
  }
  std::cout << std::endl;
  */
  
  points = new PointW[nelts];
  pool.parallel_for(fillPoints, nelts, len);
  pool.waitAll();

}

void setValuesMatrix(const int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i*size + j] = std::rand();
    }
  }
}

void setValuesMask(const int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      mask[i*size + j] = std::rand()%2;
    }
  }
  /*
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      std::cout << mask[i*size + j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size * size];
    mask = new int[size * size];
    offsets = new int[numThreads + 1];
    int nelts = 5; 

    setValuesMatrix(size);
    setValuesMask(size);

    winnow(size, nelts, numThreads);

    if (print == 1) {
      for (int i = 0; i < nelts; ++i) {
        std::cout << points[i].i << " " << points[i].j <<
          " " << points[i].weight << std::endl;
      }
    }

    delete[] matrix;
    delete[] mask;
    delete[] offsets;
    delete[] evValues;
    delete[] points;

  } else {

  }
  return 0;
}