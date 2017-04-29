#pragma once

#include<geCore/TypeRegister.h>

namespace ge{
  namespace core{
  
    class NamespaceNode{
      public:
        using Fce = std::shared_ptr<Function>;
        using Var = std::shared_ptr<Accessor>;
      protected:
        std::string _name;
        std::map<std::string,Fce>_fces;
        std::map<std::string,Var>_vars;
        std::vector<std::shared_ptr<NamespaceNode>_subNamespaces;
      public:


    };

    class Kernel{
      public:
        using Fce = std::shared_ptr<Function>;
        using Var = std::shared_ptr<Accessor>;
        using FceFactory = Fce(*)(std::shared_ptr<TypeRegister>const&);
      protected:
        std::shared_ptr<TypeRegister>_typeRegister;
        std::map<std::string,FunctionFactory>_fces;
        //typeRegister     - TypeRegister/Producer
        //variableRegister - Namespace
        //functionRegister - Produces
        //pluginRegister   - LibraryLoader
      public:
        bool registerFunction(std::string name,FunctionFactory fceFac);
        bool registerType(std::string name,std::vector<unsigned>typeDescription);
        bool registerVariable(std::string name,Var const&var);
        bool unregisterFunction(std::string name);
        bool unregisterVariable(std::string name);
        Fce const&makeFce(std::string name);
        Var const&makeVar(std::string type);
        Fce const&getFce(std::string name);
        Var const&getVar(std::string name);
        



    };
  }
}
