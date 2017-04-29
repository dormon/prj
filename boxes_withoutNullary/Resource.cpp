#include"Resource.h"
#include"TypeRegister.h"
#include"Function.h"

Resource::~Resource(){
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  this->_tr->destroyResource(this->_id,this->_data);
  this->_tr->freeResource(this->_id,this->_data);
  this->_data = nullptr;
}

void Resource::_setSignalingDirty(){
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  for(auto const&x:this->_signalingTargets)
    if(!x->_dirty){
      x->_dirty = true;
      x->_setSignalingDirty();
    }
}

void Resource::setProducer(std::shared_ptr<Function>const&f){
  PRINT_CALL_STACK(f);
  assert(this!=nullptr);
  if(this->_producer == f)return;
  if(this->_producer){
    this->_producer->_removeSignalingTarget(this);
    this->_removeSignalingSource(&*this->_producer);
  }
  this->_producer = f;
  if(this->_producer){
    this->_producer->_addSignalingTarget(this);
    this->_addSignalingSource(&*this->_producer);
  }
}

void Resource::operator()(){
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  if(this->_producer)
    (*this->_producer)();
}
