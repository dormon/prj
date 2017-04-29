#pragma once

#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>

namespace ge{
  namespace core{

    class Producer{
      public:
        Producer(SharedTypeRegister const&tr);
        SharedTypeRegister const&getTypeRegister()const;
        std::shared_ptr<Function>fce(std::string name);
        std::shared_ptr<Accessor>res(std::string type);
        std::shared_ptr<Function>nul(std::string type);
        bool registerFunction(std::string name,std::shared_ptr<FunctionFactory>const& fceFac);
      protected:
        std::map<std::string,std::shared_ptr<FunctionFactory>>_fces;
        std::shared_ptr<TypeRegister>_typeRegister;
    };
  }
}
