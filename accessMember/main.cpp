#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Cmd{
  protected:
    virtual void*_getAttribAddress(std::string){return NULL;}
  public:
    virtual void operator()(){}
    virtual ~Cmd(){}
    template<typename T>T&getAttrib(std::string name){
      return *((T*)this->_getAttribAddress(name));
    }
};

template<typename INT0_TYPE = int,typename INT1_TYPE = int,typename FLOAT0_TYPE = float,typename FLOAT1_TYPE = float>
class Neco: public Cmd{
  protected:
    void*_getAttribAddress(std::string name){
      if(name=="Int0"  )return (void*)&this->Int0  ;
      if(name=="Int1"  )return (void*)&this->Int1  ;
      if(name=="Float0")return (void*)&this->Float0;
      if(name=="Float1")return (void*)&this->Float1;
      return NULL;
    }
  public:
    INT0_TYPE   Int0  ;
    INT1_TYPE   Int1  ;
    FLOAT0_TYPE Float0;
    FLOAT1_TYPE Float1;
    Neco(){this->Int0 = 42;this->Int1 = 100;this->Float0 = 1.1f;this->Float1 = 3.14f;}
    virtual void operator()(){
      std::cout<<"Int0: "<<this->Int0<<", Int1: "<<this->Int1<<", Float0: "<<this->Float0<<", Float1: "<<this->Float1<<std::endl;
    }
    virtual ~Neco(){}
};

template<typename TARGET_TYPE = unsigned,typename BUFFER_TYPE = unsigned>
class BindBuffer: public Cmd{
  public:
    TARGET_TYPE target;
    BUFFER_TYPE buffer;
    BindBuffer(TARGET_TYPE const&target,BUFFER_TYPE const&buffer){this->target = target;this->buffer = buffer;}
    virtual void operator()(){
      std::cout<<"target: "<<this->target<<", buffer: "<<this->buffer<<std::endl;
    }
    void*getAddress(std::string name){
      if(name=="target")return (void*)&this->target;
      if(name=="buffer")return (void*)&this->buffer;
      return NULL;
    }
};

template<template<typename...>class CommandType>
class CmdAcc: public Cmd{
  private:
    Cmd*_cmd;
  public:
    CmdAcc(Cmd*cmd){this->_cmd = cmd;}
    virtual ~CmdAcc(){delete this->_cmd;}
    inline void operator()(){
      (*_cmd)();
    }
    template<typename RETURN_TYPE>RETURN_TYPE&get(std::string name);
  //template<template<typename...>class CommandType,typename T>inline T&get(std::string name);
};



/*
template<typename TARGET_TYPE,typename BUFFER_TYPE>TARGET_TYPE&CmdAcc::get<decltype(BindBuffer<TARGET_TYPE,BUFFER_TYPE>),TARGET_TYPE>(std::string name){
  return ((BindBuffer<TARGET_TYPE,BUFFER_TYPE>*)this->_cmd)->target;
}*/

int main(){
  Cmd*ptr=new Neco<int,int,float,float>();
  ptr->getAttrib<int>("Int0")=2312312;
  (*ptr)();
  delete ptr;
  return 0;
}
