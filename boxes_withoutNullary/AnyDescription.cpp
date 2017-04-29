#include"AnyDescription.h"
#include"TypeRegister.h"

std::string AnyDescription::str(std::shared_ptr<TypeRegister>const&)const{
  return keyword<Any>();
}
