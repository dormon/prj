#pragma once

#include<vector>
#include"CommandInput.h"

using CommandId = size_t;

class CommandDescription{
  public:
    enum class Type{
      ATOMIC,
      BODY  ,
      IF    ,
      WHILE ,
    }type;
    std::vector<CommandInput>inputs;
    CommandDescription(
        Type                     const&t          ,
        std::vector<CommandInput>const&inputs = {});
    virtual ~CommandDescription();
    virtual void operator()(void*const*const&)const = 0;
};

