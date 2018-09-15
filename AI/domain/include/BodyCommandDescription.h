#pragma once

#include<array>

#include<CommandDescription.h>

class BodyCommandDescription: public CommandDescription{
  public:
    std::array<CommandId,2>                       commands    ; 
    std::vector<std::array<std::vector<size_t>,2>>inputMapping;
    BodyCommandDescription(
        std::vector<CommandInput>                     const&inputs ,
        std::array<CommandId,2>                       const&cmds   ,
        std::vector<std::array<std::vector<size_t>,2>>const&mapping);
    virtual ~BodyCommandDescription();
};
