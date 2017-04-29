#include<geCore/producer.h>
#include<geCore/body.h>

using namespace ge::core;

Producer::Producer(std::shared_ptr<TypeRegister>const&tr){
  this->_typeRegister = tr;
}

SharedTypeRegister const&Producer::getTypeRegister()const{
  return this->_typeRegister;
}

std::shared_ptr<Function>Producer::fce(std::string name){
  auto ii=this->_fces.find(name);
  if(ii==this->_fces.end())return nullptr;
  return std::dynamic_pointer_cast<Function>((*this->_fces[name])(this->_typeRegister));
}

std::shared_ptr<Accessor>Producer::res(std::string type){
  return this->_typeRegister->sharedAccessor(type.c_str());
}

std::shared_ptr<Function>Producer::nul(std::string type){
  auto res=Nullary::sharedInstance(this->_typeRegister);
  res->bindOutput(this->_typeRegister->sharedAccessor(type.c_str()));
  return res;
}

bool Producer::registerFunction(std::string name,std::shared_ptr<FunctionFactory>const& fceFac){
  if(this->_fces.count(name)){
    std::cerr<<"ERROR: Producer::registerFunction("<<name<<","<<fceFac<<") - ";
    std::cerr<<"it is already registered"<<std::endl;
    return false;
  }
  this->_fces[name]=fceFac;
  return true;
}
