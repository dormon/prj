#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

class Child0;
class Child1;

class Base: public std::enable_shared_from_this<Base>{
  friend class Child0;
  friend class Child1;
  public:
    enum Type{
      BASE   = 0,
      CHILD0 = 1,
      CHILD1 = 2,
    }type;
    Base(Type const&type = BASE){this->type = type;}
    virtual ~Base(){}
    int baseData = 0;
    std::shared_ptr<Child0>toChild0()const{
      return std::dynamic_pointer_cast<Child0>(std::const_pointer_cast<Base>(this->shared_from_this()));
    }
    std::shared_ptr<Child1>toChild1()const{
      return std::dynamic_pointer_cast<Child1>(std::const_pointer_cast<Base>(this->shared_from_this()));
    }
    virtual void print()const{
      std::cout<<this->baseData<<std::endl;
    }
};

class Child0: public Base{
  public:
    Child0():Base(CHILD0){}
    virtual ~Child0(){};
    int child0Data = 0;
    virtual void print()const override{
      this->Base::print();
      std::cout<<this->child0Data<<std::endl;
    }
};

class Child1: public Base{
  public:
    Child1():Base(CHILD1){}
    virtual ~Child1(){};
    int child1Data = 0;
    virtual void print()const override{
      this->Base::print();
      std::cout<<this->child1Data<<std::endl;
    }
};



int main(){
  std::shared_ptr<Base> b = std::make_shared<Child0>();
  b->baseData = 100;
  b->toChild0()->child0Data = 123;
  b->print();
  
  return 0;
}
