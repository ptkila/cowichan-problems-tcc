#include "tbb/tbb.h"
#include <algorithm>
#include <iostream>

class PointW {

public:

  int i, j, weight;

  PointW() {
    this->i = 0;
    this->j = 0;
    this->weight = 0;
  }

  PointW(int i, int j, int weight) {
    this->i = i;
    this->j = j;
    this->weight = weight;
  }

};

typedef tbb::blocked_range<size_t> range;
static PointW* evValues;
static PointW* points;

static int* matrix;
static int* mask;
static int* countPerLine;
static int* totalCount;

static int nelts;
static int numThreads;

/*// Prefix Scan class
http://www.drdobbs.com/parallel/implementing-partition-with-prefix-scan/240003508?pgno=1

class pScan {
  int sum;
  int* const odata;
  const int* const idata;
 
public:
  pScan(int *odata_, const int *idata_) : 
    sum(0), idata(idata_), odata(odata_) {}
 
  template<typename Tag>
  void operator()(const blocked_range<int> &r, Tag) {
    int temp = sum;
    for (int i = r.begin(); i < r.end(); ++i) {
      temp = temp + idata[i];
      if(Tag::is_final_scan())
        odata[i] = temp;
    }
    sum = temp;
  }
 
  pScan(pScan& b, split) : 
    sum(0), idata(b.idata), odata(b.odata) {}
  void reverse_join(pScan& a) {sum = a.sum + sum;}
  void assign(pScan& b) {sum = b.sum;}
};*/

class PrefixSum {
  
  int sum;

public:
  
  PrefixSum(): sum(0) {}
  
  template<typename Tag>
  void operator()(const range& r, Tag) {
    int res = sum;
    size_t end = r.end();
    for (size_t i = r.begin(); i != end; ++i) {
      res += countPerLine[i];
      if (Tag::is_final_scan()) {
        totalCount[i] = res;
      }
    }
    sum = res;
  }

  PrefixSum(PrefixSum& a, tbb::split) : sum(0) {}
  void reverse_join(PrefixSum& a) {sum += a.sum;}
  void assign(PrefixSum& b) {sum = b.sum;}

};

void fillValues(const int size) {
  
  tbb::parallel_for(range(0, size), [&](const range& r) {
      size_t end = r.end();
      for (size_t i = r.begin(); i != end; ++i) {
        int count = totalCount[i];
        for (int j = 0; j < size; ++j) {
          if (mask[i*size + j]) {
            int v = matrix[i*size + j];
            evValues[count] = PointW(i, j, v);
            count++;
          }
        }
      }
  });

}

int countPoints(const int size) {

  return parallel_reduce(
    range(0, size), 0,
    [=](const range& r, int result)->int {
      size_t end = r.end();
      for (size_t i = r.begin(); i != end; ++i) {
        int cur = 0;
        for (int j = 0; j < size; ++j) {
          cur += mask[i*size + j];
        }
        countPerLine[i + 1] = cur;
        result += cur;
      }
      return result;
    },
    [](int x, int y)->int {
      return x + y;
    });

}

void fillPoints(const int len) {
  int chunk = len/ nelts;
  for(int i = 0; i < nelts; ++i) {
    points[i] = evValues[i*chunk];
  }
}

void winnow(const int size) {
  
  int len = countPoints(size);
  evValues = new PointW[len];

  PrefixSum prefixSum;
  tbb::parallel_scan(range(0, size + 1), prefixSum, tbb::auto_partitioner());

  fillValues(size);

  std::sort(evValues, evValues + len, [&](const PointW& a, const PointW& b) -> bool { 
    return a.weight < b.weight; 
  });

  nelts = rand() % len;
  if (nelts == 0) {
      nelts = 1;
  }
  points = new PointW[nelts];

  fillPoints(len);

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
}

void setThreadsNumber() {

  tbb::task_scheduler_init init(numThreads);

}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new int[size * size];
    mask = new int[size * size];
    countPerLine = new int[size + 1];
    totalCount = new int[size + 1];

    setThreadsNumber();
    setValuesMatrix(size);
    setValuesMask(size);

    winnow(size);

    if (print == 1) {
      for (int i = 0; i < nelts; ++i) {
        std::cout << points[i].i << " " << points[i].j << " " << points[i].weight << std::endl;
      }
    }

    delete[] matrix;
    delete[] mask;
    delete[] countPerLine;
    delete[] totalCount;

  } else {

  }
  return 0;
}