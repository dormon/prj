#pragma once

#include<memory>
#include<cassert>
#include<type_traits>
#include<iostream>

#include"Defines.h"

class TypeRegister;

class Resource{
  public:
    Resource(void*d,TypeId id,std::shared_ptr<TypeRegister>const&tr){
      PRINT_CALL_STACK(d,id,tr);
      assert(this!=nullptr);
      this->_data = d;
      this->_id = id;
      this->_tr = tr;
    }
    virtual ~Resource();
    TypeId getType()const{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_id;
    }
    virtual void*  getData(){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_data;
    }
    virtual void* getDataAddress(){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return &this->_data;
    }
    //*
    template<typename T>
      operator T&(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->getData()!=nullptr);
        return *(T*)this->getData();
      }
    template<typename T>
      operator T*(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->getData()!=nullptr);
        return *(T**)this->getData();
      }// */
    /*
    template<typename T,typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value,unsigned>::type = 0>
      operator T(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->getData()!=nullptr);
        return *(T*)this->getData(); 
      }
    template<typename T,typename std::enable_if<std::is_pointer<T>::value,unsigned>::type = 0>
      operator T(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return *(T*)this->getData();
      }
    template<typename T,typename std::enable_if<std::is_reference<T>::value,unsigned>::type = 0>
      operator T(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->getData()!=nullptr);
        using OT = typename std::remove_const<typename std::remove_reference<T>::type>::type;
        return *(OT*)this->getData();
      }
      // */
    template<typename T>
      T*operator&(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return (T*)this->getDataAddress();
      }
  protected:
    std::shared_ptr<TypeRegister>_tr;// = nullptr;
    void*_data;// = nullptr;
    TypeId _id;// = 0;
};

