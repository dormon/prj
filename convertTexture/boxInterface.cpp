#include"boxInterface.h"
#include<sstream>
#include"pin.h"

BoxInterface::BoxInterface(){}
std::string BoxInterface::toStr(){
  std::stringstream ss;
  bool first=true;
  for(auto x:*this){
    if(first)first=false;
    else ss<<std::endl;
    ss<<x.first<<" "<<x.second->toStr();
  }
  return ss.str();
}

