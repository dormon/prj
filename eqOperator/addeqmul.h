#pragma once

#include<iostream>

inline bool operator==(Add const&add,Mul const&mul){
  std::cout<<"Add==Mul"<<std::endl;
  return add._a+add._b==mul._a*mul._b;
}

inline bool operator==(Mul const&mul,Add const&add){
  std::cout<<"Mul==Add"<<std::endl;
  return add==mul;
}

