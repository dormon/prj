#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>
#include<chrono>

class AA{
  public:
    virtual ~AA(){}
    AA(){
      std::cerr << "AA::AA()" << std::endl;
    }
    float aAA;
    float bAA;
};

class BB: public AA{
  public:
    BB(){
      std::cerr << "BB::BB()" << std::endl;
    }
    float aBB;
    float bBB;
};

template<typename T>
class Base{
  public:
    void init(){
      a = new T();
    }
    AA*a;
    template<typename TO>
      operator TO*(){
        return (TO*)a;
      }
};

class A: public Base<AA>{
  public:
    virtual ~A(){}
};

class B: public Base<BB>{
  public:
};

int main(){
  auto aVar = new A();
  auto bVar = new B();
  aVar->init();
  bVar->init();
  delete aVar;
  delete bVar;
  return 0;
};
