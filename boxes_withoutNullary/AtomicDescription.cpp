#include"AtomicDescription.h"

void AtomicDescription::construct(void*ptr,std::shared_ptr<TypeRegister>const&)const{
  PRINT_CALL_STACK(ptr);
  assert(this!=nullptr);
  if(this->constructor)this->constructor(ptr);
}

void AtomicDescription::destroy(void*ptr,std::shared_ptr<TypeRegister>const&)const{
  PRINT_CALL_STACK(ptr);
  assert(this!=nullptr);
  if(this->destructor)this->destructor(ptr);
}


