#pragma once

#include<vector>
#include"Defines.h"
#include"TypeDescription.h"

class MemFceDescription: public TypeDescription{
  public:
    TypeId returnType;
    TypeId classType;
    std::vector<TypeId>argsTypes;
    MemFceDescription(std::string const&name,TypeId returnType,TypeId classType,std::vector<TypeId>const&args):TypeDescription(name,MEMFCE){
      PRINT_CALL_STACK(name,returnType,classType,args);
      assert(this!=nullptr);
      this->returnType = returnType;
      this->classType = classType;
      this->argsTypes = args;
    }
    ~MemFceDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription is not constructible!");}
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription is not destructible!");}
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription has no size!");return 0;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;
};


