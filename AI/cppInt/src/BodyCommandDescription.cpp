#include<cassert>

#include<BodyCommandDescription.h>

BodyCommandDescription::BodyCommandDescription(
    std::vector<CommandId>          const&cmds   ,
    std::vector<std::vector<size_t>>const&mapping):
  CommandDescription(CommandDescription::Type::BODY)
{
  static_cast<void>(cmds);
  static_cast<void>(mapping);

  assert(this != nullptr);

}

BodyCommandDescription::~BodyCommandDescription(){

}

void BodyCommandDescription::operator()(void*const*const&)const{

}

