#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<glm/glm.hpp>
#include<geCore/Text.h>

class A{
  public:
    virtual void print()=0;
};

class B: public A{
  public:
    int _a;
    B(int a):_a(a){}
    virtual void print()override{
      std::cout<<this->_a<<std::endl;
    }
};

class C: public A{
  public:
    std::string _a;
    C(std::string const&a):_a(a){}
    virtual void print()override{
      std::cout<<this->_a<<std::endl;
    }
};



int main(int,char*[]){
  A*a = new C("asd");
  B*b = dynamic_cast<B*>(a);
  if(b)b->print();
  else std::cerr<<"cannot cast C to B"<<std::endl;

  return 0;
}
