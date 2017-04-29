#include"add.h"

#include<iostream>

Add::Add(int a,int b):Int(Int::ADD){
  this->_a = a;
  this->_b = b;
}

bool Add::operator==(Add const&other)const{
  std::cout<<"Add==Add"<<std::endl;
  return (this->_a + this->_b) == (other._a + other._b);
}


