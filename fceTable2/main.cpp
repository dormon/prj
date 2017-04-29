#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>

#include"table.h"

#define DEF_FCE(name)\
void name(){std::cout<<#name<<std::endl;}

DEF_FCE(ahoj);
DEF_FCE(svete);

class MyLoader: public FunctionLoaderInterface{
  public:
    MyLoader(){}
    ~MyLoader(){}
    virtual FUNCTION_POINTER load(char const*fceName)const{
      if(std::strcmp(fceName,"a")==0)return &ahoj;
      if(std::strcmp(fceName,"b")==0)return &svete;
      return nullptr;
    }
};

int main(){
  //auto table = std::make_shared<TrapTableDecorator<DSATableDecorator<LoaderTableDecorator<TableInterface>>>>(std::make_shared<MyLoader>());
  auto table = std::make_shared<LoaderTableDecorator<TableInterface>>(std::make_shared<MyLoader>());
  auto provider = std::make_shared<FceProvider>(table);
  provider->a();
  provider->b();
  return 0;
}


