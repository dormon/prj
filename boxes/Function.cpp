#include"Function.h"
#include"TypeRegister.h"
#include"Resource.h"
#include"Nullary.h"

Function::Function(TypeId id,std::shared_ptr<TypeRegister>const&tr){
  PRINT_CALL_STACK(id,tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  this->_id = id;
  this->_tr = tr;
  for(size_t i = 0;i<tr->getNofFceArgs(id);++i)
    this->_inputs.push_back(std::make_shared<Input>(nullptr,0));
  this->_output = nullptr;
}

bool Function::bindInput(TypeId i,std::shared_ptr<Function>const&f){
  PRINT_CALL_STACK(i,f);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getArgType(i),f->getReturnType()));
  assert(i<this->_inputs.size());
  auto oldFunction = std::get<FUNCTION>(*this->_inputs.at(i));
  if(oldFunction == f)return true;
  if(oldFunction){
    oldFunction->_removeSignalingTarget(this);
    this->_removeSignalingSource(&*oldFunction);
    assert(this->_fce2Indices.count(oldFunction)!=0);
    this->_fce2Indices.at(oldFunction).erase(i);
    if(this->_fce2Indices.at(oldFunction).empty()){
      this->_fce2Indices.erase(oldFunction);
      this->_fce2FceInput.erase(oldFunction);
    }
  }
  std::get<FUNCTION>(*this->_inputs.at(i)) = f;
  std::get<TICKS>(*this->_inputs.at(i)) = f?f->_tick-1:0;
  if(f){
    f->_addSignalingTarget(this);
    this->_addSignalingSource(&*f);
    auto ii = this->_fce2Indices.find(f);
    if(ii==this->_fce2Indices.end()){
      this->_fce2Indices[f]=std::set<size_t>();
      this->_fce2FceInput[f]=this->_inputs.at(i);
    }
    this->_fce2Indices.at(f).insert(i);
  }
  this->_dirty = true;
  this->_setSignalingDirty();
  return true;
}

bool Function::bindOutput(std::shared_ptr<Resource>const&r){
  PRINT_CALL_STACK(r);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getReturnType(),r->getType()));
  this->_output = r;
  this->_dirty = true;
  this->_setSignalingDirty();
  return true;
}

bool Function::bindOutput(std::shared_ptr<Nullary>const&n){
  PRINT_CALL_STACK(n);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getReturnType(),n->getOutputData()->getType()));
  if(this->_outputNullary){
    this->_removeSignalingTarget(this->_outputNullary);
    this->_outputNullary->_removeSignalingSource(this);
  }
  if(n){
    this->_output = n->getOutputData();
    this->_outputNullary = &*n;
    this->_addSignalingTarget(this->_outputNullary);
    this->_outputNullary->_addSignalingSource(this);
  }else this->_output = nullptr;

  this->_dirty = true;
  this->_setSignalingDirty();
  return true;
}



TypeId Function::getReturnType()const{
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  return this->_tr->getFceReturnType(this->_id);
}

TypeId Function::getArgType(size_t n)const{
  PRINT_CALL_STACK(n);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  return this->_tr->getFceArgType(this->_id,n);
}


