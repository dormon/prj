#pragma once

#include"TypeRegister.h"
#include"Function.h"

class Nullary: public Function{
  public:
    Nullary(std::shared_ptr<TypeRegister>const&tr):Function(tr->getTypeId(keyword<Any()>()),tr){
      PRINT_CALL_STACK(tr);
    }
    template<typename T>
    Nullary(std::shared_ptr<TypeRegister>const&tr,T const&val):Nullary(tr){
      PRINT_CALL_STACK(tr,val);
      this->bindOutput(tr->createResource(val));
    }
    virtual void operator()()override{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      if(!this->_dirty)return;
      this->_dirty = false;
      this->_tick++;
      this->_setSignalingDirty();
    }
  protected:
    virtual bool _do()override{
      PRINT_CALL_STACK();
      return true;
    }
};


