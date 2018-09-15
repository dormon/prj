#include<VectorModelDescription.h>

VectorModelDescription::VectorModelDescription(
    std::set<ModelId>const&bases,
    ModelId          const&inner):
  ModelDescription(ModelDescription::Type::VECTOR,bases),
  inner           (inner                               )
{}


