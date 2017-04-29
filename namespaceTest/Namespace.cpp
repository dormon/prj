#include"Namespace.h"

#include<algorithm>
#include<sstream>

using namespace ge::util::sim;

std::string Namespace::_toUpper(std::string str){
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

Namespace::Namespace(std::string name){
  this->_name=name;
}

Namespace::~Namespace(){
  for(unsigned i=0;i<this->_subNamespaces.size();++i)
    delete this->_subNamespaces[i];
}

void Namespace::insertNamespace(std::string name,Namespace*nmspace){
  this->_subNamespaceNames.push_back(name);
  this->_subNamespaces.push_back(nmspace);
  this->_name2Namespace.insert(std::pair<std::string,Namespace*>(name,nmspace));
}

std::string Namespace::toStr(unsigned indentation){
  std::stringstream ss;
  std::string ind="";
  for(unsigned i=0;i<indentation;++i)ind+=" ";
  ss<<ind<<this->_name<<"{"<<std::endl;
  for(auto x:this->_name2Variable){
    ss<<ind<<"  "<<x.second->getManager()->getTypeIdName(x.second->getId())<<" "<<x.first<<" = "<<x.second->data2Str()<<std::endl;
  }
  for(unsigned n=0;n<this->_subNamespaceNames.size();++n)
    ss<<this->_subNamespaces[n]->toStr(indentation+2);
  ss<<ind<<"}"<<std::endl;
  return ss.str();
}

Namespace*Namespace::getNamespace(std::string name){
  if(this->_name2Namespace.count(name))
    return this->_name2Namespace[name];
  return NULL;
}

void Namespace::insertVariable (std::string name,std::shared_ptr<ge::core::Accessor>const&variable){
  this->_subVariableNames.push_back(name);
  this->_subVariables.push_back(variable);
  this->_name2Variable[name]=variable;
}

void Namespace::insert(std::string name,std::shared_ptr<ge::core::Accessor>const&variable){
  int pos=name.find(".");
  if(pos==0)
    throw std::string("incorect variable name - empty namespace name");
  else if(pos<0)
    this->insertVariable(name,variable);
  else{
    std::string rest=name.substr(pos+1);
    std::string namespaceName=name.substr(0,pos);
    Namespace*nextNamespace=this->getNamespace(namespaceName);
    if(nextNamespace==NULL){
      nextNamespace=new Namespace(namespaceName);
      this->insertNamespace(namespaceName,nextNamespace);
    }
    nextNamespace->insert(rest,variable);
  }
}

std::shared_ptr<ge::core::Accessor>&Namespace::getVariable (std::string name){
  return this->_name2Variable[name];
}

bool Namespace::contain(std::string name){
  return this->_name2Variable.find(name)!=this->_name2Variable.end();
}

