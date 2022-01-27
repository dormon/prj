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

class Base{
  public:
    Base(size_t n){
      data=new uint8_t[n];
    }
    ~Base(){
      delete[]data;
    }
    struct Rec{
      Rec(Base*b):b(b){}
      float  &f32(){return *(float  *)b->data;}
      private:
        Base*b;
    };
    Rec r(){return Rec{this};}
    float  &f32(){return *(float  *)data;}
  protected:
    uint8_t*data;
};

class Base2{
  public:
    Base2(size_t n):r(*this){
      data=new uint8_t[n];
    }
    ~Base2(){
      delete[]data;
    }
    struct Rec{
      Rec(Base2&t):th(t){}
      float  &f32(){return *(float  *)th.data;}
      Base2 const&th;
    }r;
  protected:
    uint8_t*data;
};

//class r{
//  public:
//    float  &f32(){return *(float  *)data;}
//
//};

class Base3{
  public:
    Base3(size_t n){
      data=new uint8_t[n];
    }
    ~Base3(){
      delete[]data;
    }
    float  &f32(){return *(float  *)data;}
  protected:
    uint8_t*data;
};

int main(){
  std::cerr << sizeof(Base) << std::endl;
  std::cerr << sizeof(Base2) << std::endl;
  auto b2=Base2(sizeof(uint32_t));
  b2.r.f32();

  auto b=Base(sizeof(uint32_t));
  b.r().f32();
  return 0;
}
