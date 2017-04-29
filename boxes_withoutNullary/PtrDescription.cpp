#include"PtrDescription.h"
#include"TypeRegister.h"
#include"PtrResource.h"

/*
std::shared_ptr<Resource>PtrDescription::createResource(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  return std::make_shared<PtrResource>(tr->getTypeId(this->name),tr);
}
*/
std::string PtrDescription::str(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  std::stringstream ss;
  ss<<"Ptr-";
  ss<<tr->typeIdName(this->innerType);
  ss<<"*";
  return ss.str();
}


