#pragma once

#include<vector>
#include"Defines.h"
#include"TypeDescription.h"

class FceDescription: public TypeDescription{
  public:
    TypeId returnType;
    std::vector<TypeId>argsTypes;
    FceDescription(std::string const&name,TypeId returnType,std::vector<TypeId>const&args):TypeDescription(name,FCE){
      PRINT_CALL_STACK(name,returnType,args);
      assert(this!=nullptr);
      this->returnType = returnType;
      this->argsTypes = args;
    }
    ~FceDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription is not constructible!");}
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription is not destructible!");}
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{assert(false&&"FceDescription has no size!");return 0;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;
};


