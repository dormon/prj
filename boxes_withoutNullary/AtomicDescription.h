#pragma once

#include"TypeDescription.h"
#include"Defines.h"

class AtomicDescription: public TypeDescription{
  public:
    size_t size = 0;
    FcePtr constructor = nullptr;
    FcePtr destructor = nullptr;
    AtomicDescription(
        std::string const&name,
        size_t s,
        FcePtr c = nullptr,
        FcePtr d = nullptr):TypeDescription(name,ATOMIC){
      PRINT_CALL_STACK(name,s,c,d);
      assert(this!=nullptr);
      this->size = s;
      this->constructor = c;
      this->destructor = d;
    }
    virtual ~AtomicDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual void destroy(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{PRINT_CALL_STACK();return this->size;}
    virtual std::string str(std::shared_ptr<TypeRegister>const&)const override{PRINT_CALL_STACK();return "Atomic";}
};


