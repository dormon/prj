#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>
#include<functional>
#include<sstream>

class A{
  public:
    float data;

};

class B: public A{
  public:
};

class C: public A{
  public:
};

class D: public B, public C{
  public:
};

int main(){
  auto d = new D();
  d->B::data = 10;
  d->C::data = 100;
  dynamic_cast<B*>(d)->data = 1313;

  std::cout<<d->B::data<<std::endl;
  std::cout<<d->C::data<<std::endl;
  std::cout<<"sizeof(A): "<<sizeof(A)<<std::endl;
  std::cout<<"sizeof(B): "<<sizeof(B)<<std::endl;
  std::cout<<"sizeof(C): "<<sizeof(C)<<std::endl;
  std::cout<<"sizeof(D): "<<sizeof(D)<<std::endl;
  return 0;
}
