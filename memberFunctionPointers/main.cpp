#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

class A{
  public:
    int c;
    A(int value){c=value;}
    int get(int a){return a+this->c;}
};

class B{
  public:
    int c;
    B(int value){c=value;}
    int add(int a,int b){return a+b+this->c;}
};

int main(){
  class Empty;
  using MPTR = void(Empty::*)();
  using APTR = int(A::*)(int);
  MPTR a=(MPTR)&A::get;
  auto va = new A(100);

  std::cout<<((Empty*)va->*((int(Empty::*)(int))a))(11)<<std::endl;

  APTR aptr = &A::get;
  std::cout<<(va->*(aptr))(10)<<std::endl;
  //ta->*((TPTR)a)(10);
  

  delete va;
  return 0;
}
