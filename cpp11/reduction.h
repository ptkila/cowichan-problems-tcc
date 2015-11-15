#include <future>
#include <mutex>
#include <vector>

template<class T> 
class Reduction {

  T dta, dtainit;
  std::promise<T> prom;
  static std::mutex m;
  static std::vector<future<T>> fut;

public:

  Reduction() throws IstoNaoPode;
  Reduction(T& dta);
  Reduction(const Reduction& r);
  ~Reduction();
  Reduction<T>& operator+=(const Reduction<T>& dta);
  T& get();
};