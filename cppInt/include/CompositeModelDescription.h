#pragma once

#include"ModelDescription.h"

class CompositeModelDescription: public ModelDescription{
  public:
    std::set<ModelId>inners;
    CompositeModelDescription(
        std::set<ModelId>const&bases ,
        std::set<ModelId>const&inners);
};

