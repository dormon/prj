#pragma once

#include<cstring>
#include"Defines.h"
#include"TypeDescription.h"

class PtrDescription: public TypeDescription{
  public:
    TypeId innerType;
    PtrDescription(std::string const&name,TypeId innerType):TypeDescription(name,PTR){
      PRINT_CALL_STACK(name,innerType);
      assert(this!=nullptr);
      this->innerType = innerType;
    }
    virtual ~PtrDescription(){
      PRINT_CALL_STACK();
    }
    //virtual std::shared_ptr<Resource>createResource(std::shared_ptr<TypeRegister>const&tr)const override;
    /*
    virtual void*allocResource(std::shared_ptr<TypeRegister>const&)const override{
      PRINT_CALL_STACK();
      return nullptr;
    }
    virtual void freeResource(void*,std::shared_ptr<TypeRegister>const&)const override{
      PRINT_CALL_STACK();
    }*/
    virtual void construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const override{
      PRINT_CALL_STACK(ptr,tr);
      assert(this!=nullptr);
      std::memset(ptr,0,this->byteSize(tr));
    }
    virtual void destroy(void*,std::shared_ptr<TypeRegister>const&)const override{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
    }
    virtual size_t byteSize(std::shared_ptr<TypeRegister>const&)const override{
      PRINT_CALL_STACK();
      return sizeof(void*);
    }
    virtual std::string str(std::shared_ptr<TypeRegister>const&tr)const override;

};


