#pragma once

#include<vector>
#include"Defines.h"
#include"TypeDescription.h"

class StructDescription: public TypeDescription{
  public:
    std::vector<TypeId>elementsTypes;
    StructDescription(std::string const&name,std::vector<TypeId>const&elements):TypeDescription(name,STRUCT){
      PRINT_CALL_STACK(name,elements);
      assert(this!=nullptr);
      this->elementsTypes = elements;
    }
    ~StructDescription(){
      PRINT_CALL_STACK();
    }
    virtual void construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual void destroy(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override;
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&tr)const override;
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;

};


