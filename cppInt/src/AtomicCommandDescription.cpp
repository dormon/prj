#include<cassert>

#include"AtomicCommandDescription.h"

AtomicCommandDescription::AtomicCommandDescription(
    AtomicCommand            const&cmd   ,
    std::vector<CommandInput>const&inputs):
  CommandDescription(CommandDescription::Type::ATOMIC,inputs),
  command(cmd                                               )
{}

AtomicCommandDescription::~AtomicCommandDescription(){
}

void AtomicCommandDescription::operator()(void*const*const&d)const{
  assert(this          != nullptr);
  assert(this->command != nullptr);
  this->command(d);
}

