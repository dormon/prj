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
#include <cassert>

#define __VARS_IMPLEMENTATION__
#include "vars.hpp"

using namespace std;

void varTests(){
  int static base = 0;
  struct CD{CD(int a){base = a;}~CD(){base = 10;}};
  {
    auto ptr = allocateVar<float>();
    deallocateVar(ptr);
  }
  {
    base = 0;
    auto ptr = allocateVar<CD>();
    callConstructor<CD>(ptr,3);
    assert(base == 3);
    auto dst = getDestructor<CD>();
    dst(ptr);
    assert(base == 10);
    deallocateVar(ptr);
  }
  {
    base = 0;
    auto ptr = construct<CD>(3);
    assert(base == 3);
    getDestructor<CD>()(ptr);
    deallocateVar(ptr);
    assert(base == 10);
  }
  {
    base = 0;
    {
      auto ptr = allocateVar<CD>();
      callConstructor<CD>(ptr,3);
      auto a = Var{ptr,getDestructor<CD>(),typeid(CD)};
      assert(base == 3);
    }
    assert(base == 10);
  }
  {
    var<float>();
  }
  {
    var<float>(1.3f);
  }
  {
    auto a = var<float>(3.f);
    a = 3.f;
  }
  {
    auto a = var<float>(3.f);
    assert(a.stamp() == 0);
    a.updateStamp();
    assert(a.stamp() == 1);
    a.reCreate<float>(4.f);
    assert(a.stamp() == 2);
    a.reCreate<uint16_t>(2);
    assert(a.stamp() == 3);
  }
}

void tests(){
  varTests();
  {
    Vars vars;
    auto a = vars.f32("a");
  }
  {
    Vars vars;
    vars.f32("a") = 32;
  }
  {
    Vars vars;
    auto a = vars.f32("a",1.f);
    assert(a == 1.f);
  }
  {
    Vars vars;
    vars.f32("a",1.f) = 3.f;
    assert(vars.f32("a") == 3.f);
  }
  {
    Vars vars;
    auto b = vars.f32("a",3.f);
    assert(b == 3.f);
    auto c = vars.f32("a",4.f);
    assert(c == 3.f);
  }
  {
    Vars vars;
    auto b = vars.f32("a",3.f);
    assert(b == 3.f);
    auto c = vars.f32("a",4.f);
    assert(c == 3.f);

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
  //auto a = var<float>();
  //std::cerr << a.stamp() << std::endl;
  //a = 3.3f;
  //std::cerr << a.stamp() << std::endl;
  //float b = a;
  return 0;
}
