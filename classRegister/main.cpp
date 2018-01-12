#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<typeinfo>
#include<typeindex>

class Base{
  public:
    Base(std::type_index t):type(t){}
    std::type_index type;
};

class Child0: public Base{
  public:
    Child0():Base(typeid(decltype(*this))){}
};

class Child1: public Base{
  public:
    Child1():Base(typeid(decltype(*this))){}
};

int main(){
  std::shared_ptr<Base>ptr;
  ptr = std::make_shared<Child0>();
  if      (ptr->type == typeid(Child0)){
  }else if(ptr->type == typeid(Child1)){
  }else{
  }
  return 0;
}


