#pragma once

#include<iostream>
#include<cassert>
#include<memory>
#include"Defines.h"

class TypeRegister;
class Resource;

class TypeDescription{
  public:
    enum TypeType{
      ATOMIC = 0,
      ARRAY  = 1,
      PTR    = 2,
      FCE    = 3,
      MEMFCE = 4,
      STRUCT = 5,
      ANY    = 6,
      VOID   = 7,
    }type;
    std::string name = "";
    TypeDescription(
        std::string const&n,
        TypeType t){
      PRINT_CALL_STACK(n,t);
      assert(this!=nullptr);
      this->name = n;
      this->type = t;
    }
    virtual ~TypeDescription(){}
    virtual std::shared_ptr<Resource>createResource(std::shared_ptr<TypeRegister>const&tr)const;
    virtual void*allocResource(std::shared_ptr<TypeRegister>const&tr)const;
    virtual void freeResource(void*ptr,std::shared_ptr<TypeRegister>const&tr)const;
    virtual void construct(void*,std::shared_ptr<TypeRegister>const&)const{___;}
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const{___;}
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const{return 0;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&)const = 0;
};


