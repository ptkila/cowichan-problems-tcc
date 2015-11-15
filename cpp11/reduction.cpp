#include "reduction.h"

template<class T> 
void Reduction<T>::Reduction() throws Invalid {
  //throw new ReductionRequiresTData();
}

template<class T> 
void Reduction<T>::Reduction( T& dta ) {
  this->dta = this->dtainit = dta;
  this->m.lock();
  this->fut.push_back(prom.get_future());
  this->m.unlock();
}

template<class T> 
void Reduction<T>::Reduction( const Reduction &r ) {
  this->dta = r.dtainit;
  this->m.lock();
  this->fut.push_back(this->prom.get_future());
  this->m.unlock();
}

template<class T> 
void Reduction<T>::~Reduction() {
  this->prom.set_future(this->dta);
}

template<class T>
Reduction<T>& Reduction<T>::operator+=( const Reduction<T>& dta ) {
  this->dta += dta;
  return *this;
}

template<class T> 
T& Reduction<T>::get() {
  T aux = this->dtainit;
  for (list<future<T>>& i : fut)
    aux += i.get_future();
  return aux;
}