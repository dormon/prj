#include"Function.h"
#include"TypeRegister.h"
#include"Resource.h"

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

bool Function::bindInput(TypeId i,std::shared_ptr<Resource>const&r){
  PRINT_CALL_STACK(i,r);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getArgType(i),r->getType()));
  assert(i<this->_inputs.size());
  auto oldResource = std::get<RESOURCE>(*this->_inputs.at(i));
  if(oldResource == r)return true;
  if(oldResource){
    oldResource->_removeSignalingTarget(this);
    this->_removeSignalingSource(&*oldResource);
    assert(this->_res2Indices.count(oldResource)!=0);
    this->_res2Indices.at(oldResource).erase(i);
    if(this->_res2Indices.at(oldResource).empty()){
      this->_res2Indices.erase(oldResource);
      this->_res2ResInput.erase(oldResource);
    }
  }
  std::get<RESOURCE>(*this->_inputs.at(i)) = r;
  std::get<TICKS>(*this->_inputs.at(i)) = r?r->_ticks-1:0;
  if(r){
    r->_addSignalingTarget(this);
    this->_addSignalingSource(&*r);
    auto ii = this->_res2Indices.find(r);
    if(ii==this->_res2Indices.end()){
      this->_res2Indices[r]=std::set<size_t>();
      this->_res2ResInput[r]=this->_inputs.at(i);
    }
    this->_res2Indices.at(r).insert(i);
  }
  this->_dirty = true;
  this->_setSignalingDirty();
  return true;
}

bool Function::bindOutputAsVariable(std::shared_ptr<Resource>const&r){
  PRINT_CALL_STACK(r);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getReturnType(),r->getType()));
  if(this->_output==r)return true;
  if(this->_output){
    if(&*this->_output->_producer==this)
      this->_output->setProducer(nullptr);
    this->_output->_removeSignalingSource(this);
    this->_removeSignalingTarget(&*this->_output);
  }
  if(r){
    this->_addSignalingTarget(&*r);
    r->_addSignalingSource(this);
  }
  this->_output = r;
  this->_dirty = true;
  this->_setSignalingDirty();
  return true;
}

bool Function::bindOutput(std::shared_ptr<Resource>const&r){
  PRINT_CALL_STACK(r);
  assert(this!=nullptr);
  assert(this->_tr!=nullptr);
  assert(this->_tr->isConvertible(this->getReturnType(),r->getType()));
  if(this->_output==r)return true;
  if(this->_output){
    if(&*this->_output->_producer==this)
      this->_output->setProducer(nullptr);
    this->_output->_removeSignalingSource(this);
    this->_removeSignalingTarget(&*this->_output);
  }
  if(r){
    this->_addSignalingTarget(&*r);
    r->_addSignalingSource(this);
    r->setProducer(this->shared_from_this());
  }
  this->_output = r;
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

void Function::_setSignalingDirty(){
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  for(auto const&x:this->_signalingTargets)
    x->_setSignalingDirty();
}

bool Function::_processInputs(){
  assert(this!=nullptr);
  bool isAnyInputChanged = false;
  for(auto const&x:this->_res2ResInput){
    auto input = x.second;
    (*std::get<RESOURCE>(*input))();
    if(std::get<TICKS>(*input) != std::get<RESOURCE>(*input)->_ticks)
      isAnyInputChanged|=true;
    std::get<TICKS>(*input) = std::get<RESOURCE>(*input)->_ticks;
  }
  return isAnyInputChanged;
}

