#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>

#define __VARS_IMPLEMENTATION__
#include "vars.hpp"

using namespace std;

void tests(){
  {
    Vars vars;
    vars.addf32("a");
  }
}

class A{
  public:
    A(){std::cerr << "A::A()" << std::endl;}
    A(A const&){std::cerr << "A::A(A const&)" << std::endl;}
    A(A&&){std::cerr << "A::A(A&&)" << std::endl;}
    ~A(){std::cerr << "A::~A()" << std::endl;}
};

template<typename T>
class MyVec{
  public:
    template<typename...ARGS>
    void emplace_back(ARGS&&...args){
      resizeIfNeeded();

      new(((T*)data)+size)T(args...);

      size++;
    }
    ~MyVec(){
      for(size_t i=0;i<size;++i)
        (((T*)data)+i)->~T();
      free(data);
    }
  protected:
    void resizeIfNeeded(){
      if(data == nullptr){
        data = malloc(sizeof(T));
        reserved = 1;
      }else{
        if(size >= reserved)
        reserved *= 2;
        data = realloc(data,sizeof(T)*reserved);
      }
    }
    
    void*  data = nullptr;
    size_t size = 0      ;
    size_t reserved = 0  ;
};

auto measure(std::function<void()>const&f){
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> dur = end-start;
  auto t = dur.count();
  return t;
}

class T{
  public:
    template<typename C>
      T(tag<C>){}
};

int main(){
  tests();

  //auto th = std::thread([](){std::cerr<<"a"<<std::endl;});
  //return 0;

  //T{tag<float>()};
  //size_t N = 1000000;
  //{
  //  auto t = measure([&](){
  //    MyVec<float>myvec;
  //    for(size_t i=0;i<N;++i)
  //      myvec.emplace_back();
  //  });
  //  std::cerr << "time: " << t << std::endl;
  //}
  //{
  //  auto t = measure([&](){
  //    std::vector<float>myvec;
  //    for(size_t i=0;i<N;++i)
  //      myvec.emplace_back();
  //  });
  //  std::cerr << "time: " << t << std::endl;
  //}

  //return 0;
  //std::vector<A>vec;
  //
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();
  //vec.emplace_back();

  //return 0;
  auto a = var<float>();
  std::cerr << a.stamp() << std::endl;
  a = 3.3f;
  std::cerr << a.stamp() << std::endl;
  float b = a;
  return 0;
}
