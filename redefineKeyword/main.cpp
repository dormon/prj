#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

int main (){
  #define A class

  A a{
  };

#define class float
  class a = 0.f;
  std::cerr << a<< std::endl;
#define return int
  return b = a;
  std::cerr << b << std::endl;
}




