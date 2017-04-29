#pragma once

#include<vector>
#include<memory>
#include<cassert>
#include<set>
#include<map>
#include<tuple>
#include"Defines.h"

class TypeRegister;
class Resource;
class Nullary;

class Function{
  public:
    Function(TypeId id,std::shared_ptr<TypeRegister>const&tr);
    virtual ~Function(){}
    virtual bool bindInput(TypeId i,std::shared_ptr<Function>const&f);
    virtual bool bindOutput(std::shared_ptr<Resource>const&r);
    virtual bool bindOutput(std::shared_ptr<Nullary>const&n);
    virtual TypeId getReturnType()const;
    virtual TypeId getArgType(size_t n)const;
    virtual std::shared_ptr<Resource>const&getOutputData()const{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_output;
    }
    virtual std::shared_ptr<Resource>const&getInputData(size_t i)const{
      PRINT_CALL_STACK(i);
      assert(this!=nullptr);
      assert(i<this->_inputs.size());
      assert(std::get<FUNCTION>(*this->_inputs.at(i))!=nullptr);
      return std::get<FUNCTION>(*this->_inputs.at(i))->getOutputData();
    }
    TypeId getType()const{
      PRINT_CALL_STACK();
      return this->_id;
    }
    virtual void operator()(){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      if(!this->_dirty)return;
      bool isAnyInputChanged = this->_processInputs();
      if(!isAnyInputChanged){
        this->_dirty = false;
        return;
      }
      bool isOutputChanged = this->_do();
      this->_dirty = false;
      if(isOutputChanged){
        this->_tick++;
        this->_setSignalingDirty();
      }
    }
  protected:
    Nullary*_outputNullary = nullptr;
    std::set<Function*>_signalingSources;
    std::set<Function*>_signalingTargets;
    void _addSignalingSource(Function*f){
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingSources.insert(f);
    }
    void _removeSignalingSource(Function*f){
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingSources.erase(f);
    }
    void _addSignalingTarget(Function*f){
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingTargets.insert(f);
    }
    void _removeSignalingTarget(Function*f){
      assert(this!=nullptr);
      assert(f!=nullptr);
      this->_signalingTargets.erase(f);
    }
    void _setSignalingDirty(){
      for(auto const&x:this->_signalingTargets)
        if(!x->_dirty){
          x->_dirty = true;
          x->_setSignalingDirty();
        }
    };
    virtual bool _do() = 0;
    virtual bool _processInputs(){
      assert(this!=nullptr);
      bool isAnyInputChanged = false;
      for(auto const&x:this->_fce2FceInput){
        auto input = x.second;
        (*std::get<FUNCTION>(*input))();
        if(std::get<TICKS>(*input) != std::get<FUNCTION>(*input)->_tick)
          isAnyInputChanged|=true;
        std::get<TICKS>(*input) = std::get<FUNCTION>(*input)->_tick;
      }
      return isAnyInputChanged;
    }

    using Input = std::tuple<std::shared_ptr<Function>,size_t>;
    enum InputParts{FUNCTION=0,TICKS=1};
    std::vector<std::shared_ptr<Input>>_inputs;
    std::map<std::shared_ptr<Function>,std::set<size_t>>_fce2Indices;
    std::map<std::shared_ptr<Function>,std::shared_ptr<Input>>_fce2FceInput;

    size_t _tick = 1;
    bool _dirty = false;
    std::shared_ptr<Resource>_output;
    size_t _id;
    std::shared_ptr<TypeRegister>_tr;
};


