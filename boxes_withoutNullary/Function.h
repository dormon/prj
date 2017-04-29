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

class Function: public std::enable_shared_from_this<Function>{
  friend class Resource;
  public:
    Function(TypeId id,std::shared_ptr<TypeRegister>const&tr);
    virtual ~Function(){}
    virtual bool bindInput(TypeId i,std::shared_ptr<Resource>const&r);
    virtual bool bindOutput(std::shared_ptr<Resource>const&r);
    virtual bool bindOutputAsVariable(std::shared_ptr<Resource>const&r);
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
      return std::get<RESOURCE>(*this->_inputs.at(i));
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
        this->_setSignalingDirty();
      }
    }
  protected:
    std::set<Resource*>_signalingSources;
    std::set<Resource*>_signalingTargets;
    using Input = std::tuple<std::shared_ptr<Resource>,Ticks>;
    enum InputParts{RESOURCE=0,TICKS=1};
    std::vector<std::shared_ptr<Input>>_inputs;
    std::map<std::shared_ptr<Resource>,std::set<size_t>>_res2Indices;
    std::map<std::shared_ptr<Resource>,std::shared_ptr<Input>>_res2ResInput;

    bool _dirty = false;
    std::shared_ptr<Resource>_output;
    TypeId _id;
    std::shared_ptr<TypeRegister>_tr;

    void _setSignalingDirty();
    virtual bool _do() = 0;
    virtual bool _processInputs();
    void _addSignalingSource(Resource*r){
      PRINT_CALL_STACK(r);
      assert(this!=nullptr);
      assert(r!=nullptr);
      this->_signalingSources.insert(r);
    }
    void _removeSignalingSource(Resource*r){
      PRINT_CALL_STACK(r);
      assert(this!=nullptr);
      assert(r!=nullptr);
      this->_signalingSources.erase(r);
    }
    void _addSignalingTarget(Resource*r){
      PRINT_CALL_STACK(r);
      assert(this!=nullptr);
      assert(r!=nullptr);
      this->_signalingTargets.insert(r);
    }
    void _removeSignalingTarget(Resource*r){
      PRINT_CALL_STACK(r);
      assert(this!=nullptr);
      assert(r!=nullptr);
      this->_signalingTargets.erase(r);
    }
};


