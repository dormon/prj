#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geGL/geGL.h>
#include<GL/glew.h>
#include"attrib.h"

/// HEADERFILE

class RegisterImpl;

class Register{
  public:
    Register();
    void add();
    void get();
    void print();
  protected:
    std::unique_ptr<RegisterImpl>_impl;
};

/// CPP

class RegisterImpl{
  public:
    int a;
    float b;
    void createInfo();
};

Register::Register(){
  this->_impl = std::make_unique<RegisterImpl>([]{})
}


int main(){
  return 0;
}





