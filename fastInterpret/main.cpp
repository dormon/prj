#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>

#include"macros.h"

// powerset real integer;
// powerset real2 real;
//
// struct Name{
//   type   name;
//   number name;
//   bool   name;
//   real   name;
// };
//
// array type[3] Name;
//
//
//
// name(in type name,out type name,inout type name){
//   type name;
//   fce(name,name,name);
//   if(name)
//     fce(name);
//   while(name){
//     fce(name,name);
//     fce(name,name,name);
//   }
// }
//
//
//
//
//

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
