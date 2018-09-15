#pragma once

#include"ModelDescription.h"

class AtomicModelDescription: public ModelDescription{
  public:
    AtomicModelDescription(
        std::set<ModelId>const&bases);
};


