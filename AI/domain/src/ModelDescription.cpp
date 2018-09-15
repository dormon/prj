#include"ModelDescription.h"

ModelDescription::ModelDescription(
    Type             const&t,
    std::set<ModelId>const&b):
  type (t),
  bases(b)
{}



