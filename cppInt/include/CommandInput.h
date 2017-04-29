#pragma once

#include"ModelDescription.h"

class CommandInput{
  public:
    ModelId type;
    enum class AccessType{
      COMMAND_READS            = 1<<0                          ,
      COMMAND_WRITES           = 1<<1                          ,
      COMMAND_READS_AND_WRITES = COMMAND_READS | COMMAND_WRITES,
    }access;
    CommandInput(
        ModelId    const&t,
        AccessType const&a);
};

