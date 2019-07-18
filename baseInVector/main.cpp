#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

class Base{
  public:
    virtual void print()const = 0;
};

class A:public Base{
  public:
    void print()const override{
      std::cerr << "A::print()" << std::endl;
    }
};

class B:public Base{
  public:
    void print()const override{
      std::cerr << "B::print()" << std::endl;
    }
};

int main(){
  std::vector<Base>data;
  //data.push_back(B());
  //data.push_back(A());
  //data.push_back(B());
  //for(auto const&d:data)
  //  d.print();
  return 0;
}


