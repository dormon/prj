#pragma once

#include"TypeDescription.h"

class AnyDescription: public TypeDescription{
  public:
    AnyDescription(std::string const&name):TypeDescription(name,ANY){
      PRINT_CALL_STACK(name);
    }
    ~AnyDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"AnyDescription is not constructible!");}
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"AnyDescription is not destructible!");}
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{assert(false&&"AnyDescription has no size!");return 0;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&)const override;
};


