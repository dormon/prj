#pragma once

#include<cstring>
#include"Defines.h"
#include"TypeDescription.h"

class VoidDescription: public TypeDescription{
  public:
    VoidDescription(std::string const&name):TypeDescription(name,VOID){
      PRINT_CALL_STACK(name);
      assert(this!=nullptr);
    }
    virtual ~VoidDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"VoidDescription is not constructible!");}
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const override{assert(false&&"VoidDescription is not destructible!");}
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{assert(false&&"VoidDescription has no size!");return 0;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;

};


