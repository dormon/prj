#include"CompositeModelDescription.h"

CompositeModelDescription::CompositeModelDescription(
    std::set<ModelId>const&base  ,
    std::set<ModelId>const&inners):
  ModelDescription(ModelDescription::Type::COMPOSITE,base),
  inners          (inners                                )
{}

