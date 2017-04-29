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
    void operator()(){
      PRINT_CALL_STACK();
      for(auto const&x:this->_inputs)(*std::get<FUNCTION>(*x))();
      auto o = this->getOutputData();
      auto i = this->getInputData(0);
      size_t n = this->_tr->getByteSize(this->_tr->getFceReturnType(this->_id));
      std::memcpy(o->getData(),*(void**)i->getData(),n);
    }
};


