#include"geometry.h"

template<>void Uniform<1,float>::operator()(){
  this->_data[0]=0;
}
template<>void Uniform<2,float>::operator()(){
  this->_data[0]=0;
  this->_data[0]=1;
}
