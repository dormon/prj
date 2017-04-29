#pragma once

#include"CommandDescription.h"

using AtomicCommand = void(*)(void*const*const&);

class AtomicCommandDescription: public CommandDescription{
  public:
    AtomicCommand command;
    AtomicCommandDescription(
        AtomicCommand            const&cmd   ,
        std::vector<CommandInput>const&inputs);
    virtual ~AtomicCommandDescription();
    virtual void operator()(void*const*const&d)const override;
};

