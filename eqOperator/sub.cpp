#include"sub.h"

#include<iostream>

Sub::Sub(int a,int b):Int(Int::SUB){
  this->_a = a;
  this->_b = b;
}

bool Sub::operator==(Sub const&other)const{
  std::cout<<"Sub==Sub"<<std::endl;
  return (this->_a - this->_b) == (other._a - other._b);
}


