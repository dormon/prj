#include"Resource.h"
#include"TypeRegister.h"

Resource::~Resource(){
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  this->_tr->destroyResource(this->_id,this->_data);
  this->_tr->freeResource(this->_id,this->_data);
  this->_data = nullptr;
}


