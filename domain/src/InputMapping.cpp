#include<cassert>

#include"InputMapping.h"

InputMapping::InputMapping(std::vector<std::vector<size_t>>const&mapping){
  assert(this != nullptr);
  this->addSubCommands(mapping);
}

void InputMapping::addSubCommands(std::vector<std::vector<size_t>>const&mapping){
  assert(this != nullptr);
  for(auto const&x:mapping)
    this->addSubCommand(x);
}

void InputMapping::addSubCommand(std::vector<size_t>const&mapping){
  assert(this != nullptr);
}

size_t InputMappin::getNofSubCommands()const{
  assert(this != nullptr);

}

size_t getInputForSubCommand         (
    size_t const&subCommandIndex     ,
    size_t const&subCommandInputIndex)const;
