#pragma once

#include"TypeRegister.h"
#include"Function.h"
#include"Resource.h"

class Dereference: public Function{
  public:
    template<typename T>
    Dereference(std::shared_ptr<TypeRegister>const&tr):Function(tr->addType<T(T*)>(),tr){
      PRINT_CALL_STACK(tr);
    }
  protected:
    virtual bool _do()override{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      assert(this->_tr!=nullptr);
      auto o = this->getOutputData();
      auto i = this->getInputData(0);
      size_t n = this->_tr->getByteSize(this->_tr->getFceReturnType(this->_id));
      assert(o!=nullptr);
      assert(i!=nullptr);
      assert(i->getData()!=nullptr);
      std::memcpy(o->getData(),*(void**)i->getData(),n);
      return true;
    }
};


