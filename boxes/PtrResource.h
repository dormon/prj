#pragma once

#include"Resource.h"

class PtrResource: public Resource{
  public:
    PtrResource(TypeId id,std::shared_ptr<TypeRegister>const&tr):Resource(nullptr,id,tr){
      PRINT_CALL_STACK(id,tr);
    }
    /*
    virtual ~PtrResource(){
      PRINT_CALL_STACK();
    }*/
    /*
    virtual void* getData()override{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      assert(this->_data!=nullptr);
      return *(void**)this->_data;
    }
    virtual void* getDataAddress()override{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_data;
    }
    */
};
