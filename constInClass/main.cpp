#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>

class A{
  public:
    int a;
    int const b = 1;
};

class B{
  public:
    int a;
    int b;
};

class C{
  public:
    int a;
    static int const b = 12;
};

int main(){
  std::cout<<"sizeof(A): "<<sizeof(A)<<std::endl;
  std::cout<<"sizeof(B): "<<sizeof(B)<<std::endl;
  std::cout<<"sizeof(C): "<<sizeof(C)<<std::endl;
  return 0;
}
