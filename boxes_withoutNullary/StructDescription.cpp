#include"StructDescription.h"
#include"TypeRegister.h"

void StructDescription::construct(void*ptr,std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(ptr,tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  size_t offset = 0;
  auto pptr = (uint8_t*)ptr;
  for(auto const&x:this->elementsTypes){
    tr->constructResource(x,pptr+offset);
    offset += tr->getByteSize(x);
  }
}

void StructDescription::destroy(void*ptr,std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(ptr,tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  size_t offset = 0;
  auto pptr = (uint8_t*)ptr;
  for(auto const&x:this->elementsTypes){
    tr->destroyResource(x,pptr+offset);
    offset += tr->getByteSize(x);
  }
}

size_t StructDescription::byteSize(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  size_t size = 0;
  for(auto const&x:this->elementsTypes)size += tr->getByteSize(x);
  return size;
}

std::string StructDescription::str(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  std::stringstream ss;
  ss<<"Struct-{";
  bool first = true;
  for(auto const&x:this->elementsTypes){
    if(first)first = false;
    else ss<<",";
    ss<<tr->typeIdName(x);
  }
  ss<<"}";
  return ss.str();
}


