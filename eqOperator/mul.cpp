#include"mul.h"

#include<iostream>

Mul::Mul(int a,int b):Int(Int::MUL){
  this->_a = a;
  this->_b = b;
}

bool Mul::operator==(Mul const&other)const{
  std::cout<<"Mul==Mul"<<std::endl;
  return (this->_a * this->_b) == (other._a * other._b);
}


