#include"CommandDescription.h"

CommandDescription::CommandDescription(
    Type                     const&t,
    std::vector<CommandInput>const&i):
  type  (t),
  inputs(i)
{}

CommandDescription::~CommandDescription(){}

