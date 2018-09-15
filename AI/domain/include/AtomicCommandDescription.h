#pragma once

#include<CommandDescription.h>

class AtomicCommandDescription: public CommandDescription{
  public:
    AtomicCommandDescription(
        std::vector<CommandInput>const&inputs);
    virtual ~AtomicCommandDescription();
};

