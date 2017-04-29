#pragma once

#include<memory>
#include<cassert>
#include<type_traits>
#include<iostream>
#include<set>

#include"Defines.h"

class TypeRegister;
class Function;

class Resource{
  friend class Function;
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
      }
    template<typename T>
      T*operator&(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return (T*)this->getDataAddress();
      }
    Ticks getTicks()const{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_ticks;
    };
    void updateTicks(){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      this->_ticks++;
    }
    template<typename T>
      void update(T const&value){
        PRINT_CALL_STACK(value);
        assert(this!=nullptr);
        (T&)*this = value;
        this->updateTicks();
      }
    void setProducer(std::shared_ptr<Function>const&f);
  protected:
    virtual void operator()();
    void _addSignalingTarget(Function*f){
      PRINT_CALL_STACK(f);
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingTargets.insert(f);
    }
    void _removeSignalingTarget(Function*f){
      PRINT_CALL_STACK(f);
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingTargets.erase(f);
    }
    void _addSignalingSource(Function*f){
      PRINT_CALL_STACK(f);
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingSources.insert(f);
    }
    void _removeSignalingSource(Function*f){
      PRINT_CALL_STACK(f);
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingSources.erase(f);
    }
    void _setSignalingDirty();
    std::set<Function*>_signalingTargets;
    std::set<Function*>_signalingSources;
    std::shared_ptr<Function>_producer = nullptr;
    Ticks _ticks = 1;
    std::shared_ptr<TypeRegister>_tr;// = nullptr;
    void*_data;// = nullptr;
    TypeId _id;// = 0;
};

