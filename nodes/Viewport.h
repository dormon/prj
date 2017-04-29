#pragma once

#include<Layer.h>

class Viewport{
  public:
    virtual~Viewport();
    std::vector<Layer*>layers;
};

inline Viewport::~Viewport(){
  ___;
  assert(this!=nullptr);
  for(auto x:this->layers)
    delete x;
  ___;
}
