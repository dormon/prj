#include"producer.h"

Producer::Producer(std::shared_ptr<ge::core::TypeRegister>const&tr){
  this->_typeRegister = tr;
}

std::shared_ptr<ge::core::Function>Producer::fce(std::string name){
  auto ii=this->_fces.find(name);
  if(ii==this->_fces.end())return nullptr;
  return this->_fces[name](this->_typeRegister);
}

std::shared_ptr<ge::core::Accessor>Producer::res(std::string type){
  return this->_typeRegister->sharedAccessor(type.c_str());
}

std::shared_ptr<ge::core::Function>Producer::nul(std::string type){
  auto res=ge::core::Nullary::sharedInstance(this->_typeRegister);
  res->bindOutput(this->_typeRegister->sharedAccessor(type.c_str()));
  return res;
}

bool Producer::registerFunction(std::string name,FunctionFactory fceFac){
  if(this->_fces.count(name)){
    std::cerr<<"ERROR: Producer::registerFunction("<<name<<","<<fceFac<<") - ";
    std::cerr<<"it is already registered"<<std::endl;
    return false;
  }
  this->_fces[name]=fceFac;
  return true;
}
