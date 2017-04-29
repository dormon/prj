#include"TypeDescription.h"
#include"TypeRegister.h"
#include"Resource.h"

void*TypeDescription::allocResource(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  return new uint8_t[this->byteSize(tr)];
}

void TypeDescription::freeResource(void*ptr,std::shared_ptr<TypeRegister>const&)const{
  PRINT_CALL_STACK(ptr);
  assert(this!=nullptr);
  delete[](uint8_t*)ptr;
}


std::shared_ptr<Resource>TypeDescription::createResource(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  auto id = tr->getTypeId(this->name);
  auto d = this->allocResource(tr);
  assert(d!=nullptr);
  this->construct(d,tr);
  return std::make_shared<Resource>(d,id,tr);
}


