#include"VoidDescription.h"

std::string VoidDescription::str(std::shared_ptr<TypeRegister>const&)const{
  PRINT_CALL_STACK();
  assert(this!=nullptr);
  return "Void-"+this->name;
}
 
