#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>

#include<geGL/TableInterface.h>
#include<geGL/LoaderTableDecorator.h>
#include<geGL/DSATableDecorator.h>
#include<geGL/TrapTableDecorator.h>
#include<geGL/FunctionProvider.h>



#define DEF_FCE(name)\
void name(){std::cout<<#name<<std::endl;}

DEF_FCE(ahoj);
DEF_FCE(svete);

class MyLoader: public ge::gl::opengl::FunctionLoaderInterface{
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
  auto table = std::make_shared<ge::gl::opengl::TrapTableDecorator<ge::gl::opengl::DSATableDecorator<ge::gl::opengl::LoaderTableDecorator<ge::gl::opengl::TableInterface>>>>(std::make_shared<MyLoader>());
  table->construct();
  auto provider = std::make_shared<ge::gl::opengl::FunctionProvider>(table);
  provider->a();
  provider->b();
  return 0;
}


