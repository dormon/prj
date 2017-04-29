#include<cassert>

#include<BodyCommandDescription.h>

BodyCommandDescription::BodyCommandDescription(
    std::vector<CommandInput>       const&inputs ,
    std::vector<CommandId>          const&cmds   ,
    std::vector<std::vector<size_t>>const&mapping):
  CommandDescription(CommandDescription::Type::BODY,inputs),
  commands          (cmds                                 ),
  inputMapping      (mapping                              )
{}

BodyCommandDescription::~BodyCommandDescription(){}

