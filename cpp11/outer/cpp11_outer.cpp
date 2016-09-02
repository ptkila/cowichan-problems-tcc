  #include <iostream>
  #include <cstdlib>
  #include <climits>
  #include <ctime>
  #include <cmath>
  #include <thread>
  #include <mutex>
  #include <vector>
  #include <functional>
  #include <future>

class Point {

public:

  double x;
  double y;

  Point () {
    this->x = 0.0;
    this->y = 0.0;
  }

  Point (double x, double y) {
    this->x = x;
    this->y = x;
  }
};

static double* matrix;
static double* vector;
static Point* points;
static int numThreads;

double sqr(const double x) {

  return x * x;

}

double distance(const class Point a, const class Point b) {

  return sqrt(sqr(a.x - a.y) + sqr(b.x - b.y));

}

void calc (const int startLine, const int endLine, const int size) {
  
  Point origin = new Point();

  for (int i = startLine; i < endLine; i++) {
    double nMax = 0;
    for (int j = 0; j < size; j++) {
      if (i != j) {
        matrix[i * size + j] = distance(points[i], points[j]);
        nMax = fmax(nMax, matrix[i][j]);
      }
    }
    matrix[i*size + i] = nMax * size;
    vector[i] = distance(origin, points[i]);
  }
}

void outer(const int size) {

  int numOpThreadM = (int)floor((float)size / (float)numThreads);
  int numOpThreadR = size % numThreads;
  std::thread threadsList[numThreads];

  for (int i = 0; i < numThreads; ++i) {
    if (i + 1 == numThreads && numOpThreadR > 0) {
      threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1) + numOpThreadR, 
        size));
      break;
    } else {
      threadsList[i] = (std::thread(calc, numOpThreadM * i, numOpThreadM * (i + 1), size));
    }
  }

  for ( auto &t : threadsList ) {
    t.join();
  }

}

void setPointValues(const int size) {
  for (int i = 0; i < size; i++) {
    points[i].x = rand();
    points[i].y = rand();
  }
}

int main(int argc, char** argv) {

  if (argc == 4) {

    srand (time(NULL));
    int size = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    int print = atoi(argv[3]);

    matrix = new double[size * size];
    vector = new double[size];
    points = new Point[size];

    setPointValues(size);
    outer(size);

    if (print == 1)
    {
      std::cout.precision(0);

      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          std::cout << matrix[i*size + j] << " ";
        }
        std::cout << "\n";
      }

      for (int i = 0; i < size; i++) {
        std::cout << vector[i] << " ";
      }

      std::cout << "\n";
    }

    delete[] matrix;
    delete[] vector;
    delete[] points;

  } else {


  }

  return 0;
}