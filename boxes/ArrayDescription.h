#pragma once

#include"Defines.h"
#include"TypeDescription.h"

class ArrayDescription: public TypeDescription{
  public:
    TypeId innerType;
    size_t size;
    ArrayDescription(std::string const&name,TypeId innerType,size_t size):TypeDescription(name,ARRAY){
      PRINT_CALL_STACK(name,innerType,size);
      assert(this!=nullptr);
      this->innerType = innerType;
      this->size = size;
    }
    virtual ~ArrayDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual void destroy(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&tr)const override;
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;
  private:
};


