#include<cassert>

#include<BodyCommandDescription.h>

BodyCommandDescription::BodyCommandDescription(
    std::vector<CommandInput>                     const&inputs ,
    std::array<CommandId,2>                       const&cmds   ,
    std::vector<std::array<std::vector<size_t>,2>>const&mapping):
  CommandDescription(CommandDescription::Type::BODY,inputs),
  commands          (cmds                                 ),
  inputMapping      (mapping                              )
{}

BodyCommandDescription::~BodyCommandDescription(){}

