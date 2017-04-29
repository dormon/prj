#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<geCore/TypeRegister.h>

namespace ge{
  namespace util{
    namespace sim{
      class Namespace{
        protected:
          std::string _name;
          std::vector<std::string>        _subNamespaceNames;
          std::vector<Namespace*>         _subNamespaces    ;
          std::vector<std::string>        _subVariableNames ;
          std::map<std::string,Namespace*>_name2Namespace   ;
          std::vector<std::shared_ptr<ge::core::Accessor>> _subVariables;
          std::map<std::string,std::shared_ptr<ge::core::Accessor>> _name2Variable;
          std::string _toUpper(std::string str);
        public:
          Namespace(std::string name);
          virtual ~Namespace();
          void insertNamespace(std::string name,Namespace*nmspace);
          void insertVariable (std::string name,std::shared_ptr<ge::core::Accessor>const&variable);
          void insert(std::string name,std::shared_ptr<ge::core::Accessor>const&variable);
          std::string toStr(unsigned indentation);
          Namespace*getNamespace(std::string name);
          std::shared_ptr<ge::core::Accessor>&getVariable(std::string name);
          bool contain(std::string name);
          template<typename TYPE>
            TYPE&get(std::string name){
              return (TYPE&)(*this->getVariable(name));
            }
      };
    }
  }
}
