#pragma once

#include"Function.h"
#include"TypeRegister.h"
#include"Resource.h"

template<typename T>
class Reference: public Function{
  public:
    Reference(std::shared_ptr<TypeRegister>const&tr):Function(tr->addType<T*(T)>(),tr){
      PRINT_CALL_STACK(tr);
    }
  protected:
    virtual bool _do(){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      auto o = this->getOutputData();
      auto i = this->getInputData(0);
      assert(o!=nullptr);
      assert(i!=nullptr);
      assert(o->getData()!=nullptr);
      assert(i->getDataAddress()!=nullptr);
      std::memcpy(o->getData(),i->getDataAddress(),sizeof(void*));
      return true;
    }
};


