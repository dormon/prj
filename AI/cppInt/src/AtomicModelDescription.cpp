#include"AtomicModelDescription.h"

AtomicModelDescription::AtomicModelDescription(
    std::set<ModelId>const&bases):
  ModelDescription(ModelDescription::Type::ATOMIC,bases)
{}

