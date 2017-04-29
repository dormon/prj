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

template<typename X>
class Base{
  public:
  protected:
    X   x;
    void print(){}
    int a;
    using Type = X;
};

template<typename X>
class Child: public Base<X>{
  public:
    void delej(){
      //print(); //error 
      this->print();
      //a = 3; //error
      //Type x; //error
      
    }
};

int main(){
  return 0;
}
