#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>
#include"AllContainer.h"

class A{
  public:
    int data;
    A(int a):data(a){}
    ~A(){}
};

int main(){
  AllContainer data;
  data.addValue<A>(11);
  data.addValue<int>(1);
  data.addValue<int>(32);
  if(data.hasType<int>()){
    std::cout << *data.getValue<int>(0) << std::endl;
    std::cout << *data.getValue<int>(1) << std::endl;
  }
  return 0;
}
