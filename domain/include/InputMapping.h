#pragma once

#include<vector>

class InputMapping{
  public:
    InputMapping(size_t const&nofInputs,std::vector<std::vector<size_t>>const&mapping = {});
    void addSubCommands(std::vector<std::vector<size_t>>const&mapping);
    void addSubCommand(std::vector<size_t>const&mapping);
    size_t getNofSubCommands()const;
    size_t getInputForSubCommand         (
        size_t const&subCommandIndex     ,
        size_t const&subCommandInputIndex)const;
    std::vector<std::vector<std::vector<size_t>>>inputToSubCommandToInput;
};
