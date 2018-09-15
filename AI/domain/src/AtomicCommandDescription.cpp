#include<cassert>

#include<AtomicCommandDescription.h>

AtomicCommandDescription::AtomicCommandDescription(
    std::vector<CommandInput>const&inputs):
  CommandDescription(CommandDescription::Type::ATOMIC,inputs)
{}

AtomicCommandDescription::~AtomicCommandDescription(){
}

