#pragma once

#include<ModelDescription.h>

class VectorModelDescription: public ModelDescription{
  public:
    ModelId inner;
    VectorModelDescription(
        std::set<ModelId>const&bases,
        ModelId          const&inner);
};


