#include<iostream>
#include"base.h"

extern "C"{
  class AssimpLoader: public Loader{
    public:
      virtual ~AssimpLoader(){std::cout<<"vola se assimploader destruktor"<<std::endl;}
      virtual void print(){
        std::cout<<"toto je assimp loader"<<std::endl;
      }
  };
  Loader*getLoader(){
    return new AssimpLoader();
  }
  void printAhoj(){
    std::cout<<"ahoj svete"<<std::endl;
  }
}
