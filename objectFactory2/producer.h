#pragma once

#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>

class Producer{
  public:
    using FunctionFactory = std::shared_ptr<ge::core::Function>(*)(std::shared_ptr<ge::core::TypeRegister>const&);
    Producer(std::shared_ptr<ge::core::TypeRegister>const&tr);
    std::shared_ptr<ge::core::Function>fce(std::string name);
    std::shared_ptr<ge::core::Accessor>res(std::string type);
    std::shared_ptr<ge::core::Function>nul(std::string type);
    bool registerFunction(std::string name,FunctionFactory fceFac);
  protected:
    std::map<std::string,FunctionFactory>_fces;
    std::shared_ptr<ge::core::TypeRegister>_typeRegister;
};


