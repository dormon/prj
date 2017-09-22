#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>

#include"macros.h"

class Variable{
  public:
};

class Statement{
  public:
    virtual ~Statement(){}
    virtual void operator()() = 0;
};

class Body: public Statement{
  public:
    std::vector<Statement*>statements;
    std::vector<Variable*>variables;
    virtual ~Body(){
      for(auto const&x:statements)delete x;
      for(auto const&x:variables)delete x;
    }
    virtual void operator()()override{
      for(auto const&x:statements)
        (*x)();
    }
};

class Function{
  public:
    Variable*output;
    std::vector<Variable*>inputs;
};



int main(){
  return 0;
}
