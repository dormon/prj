#pragma once

#include<cstdlib>
#include<set>

using ModelId = size_t;

class ModelDescription{
  public:
    enum class Type{
      ATOMIC   ,
      VECTOR   ,
      COMPOSITE,
    }type;
    std::set<ModelId>bases;
    ModelDescription(
        Type             const&t,
        std::set<ModelId>const&b);
};


