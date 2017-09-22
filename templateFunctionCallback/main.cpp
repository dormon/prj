#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>

#include"macros.h"

template<typename>void printA       ();
template<        >void printA<float>(){std::cout << "float" << std::endl;}
template<        >void printA<int  >(){std::cout << "int"   << std::endl;}

template<typename>void printB       ();
template<        >void printB<float>(){std::cout << "floatB" << std::endl;}
template<        >void printB<int  >(){std::cout << "intB"   << std::endl;}


template<typename T>
using TemplatedFunctionPointer = void(*)();

template<typename T>
void call(TemplatedFunctionPointer<T>ptr){
  ptr();
}

int main(){
  call<float>(printA<float>);
  return 0;
}
