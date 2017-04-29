#include"FceDescription.h"
#include"TypeRegister.h"

std::string FceDescription::str(std::shared_ptr<TypeRegister>const&tr)const{
  PRINT_CALL_STACK(tr);
  assert(this!=nullptr);
  assert(tr!=nullptr);
  std::stringstream ss;
  ss<<"Fce-(";
  bool first = true;
  for(auto const&x:this->argsTypes){
    if(first)first = false;
    else ss<<",";
    ss<<tr->typeIdName(x);
  }
  ss<<")->";
  ss<<tr->typeIdName(this->returnType);
  return ss.str();
}


