#include"ArrayDescription.h"
#include"TypeRegister.h"

size_t ArrayDescription::byteSize(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  return this->size*tr->getByteSize(this->innerType);
}

void ArrayDescription::construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(ptr,tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  auto offset = tr->getByteSize(this->innerType);
  auto pptr = (uint8_t*)ptr;
  for(size_t i=0;i<this->size;++i)
    tr->constructResource(this->innerType,pptr+offset*i);
}

void ArrayDescription::destroy(void*ptr,std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(ptr,tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  auto offset = tr->getByteSize(this->innerType);
  auto pptr = (uint8_t*)ptr;
  for(size_t i=0;i<this->size;++i)
    tr->destroyResource(this->innerType,pptr+offset*i);
}

std::string ArrayDescription::str(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  std::stringstream ss;
  ss<<"Array-{";
  ss<<tr->typeIdName(this->innerType);
  ss<<"}[";
  ss<<this->size;
  ss<<"]";
  return ss.str();
}



