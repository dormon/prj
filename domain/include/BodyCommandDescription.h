#pragma once

#include<CommandDescription.h>

class BodyCommandDescription: public CommandDescription{
  public:
    std::vector<CommandId>          commands    ;
    std::vector<std::vector<size_t>>inputMapping;
    BodyCommandDescription(
        std::vector<CommandInput>       const&inputs ,
        std::vector<CommandId>          const&cmds   ,
        std::vector<std::vector<size_t>>const&mapping);
    virtual ~BodyCommandDescription();
};
