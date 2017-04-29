#pragma once

#include<Node.h>

class Viewport;
class Layer{
  public:
    Layer(Node*root = nullptr);
    virtual ~Layer();
    Node*root = nullptr;
};

inline Layer::Layer(Node*r):root(r){}

inline Layer::~Layer(){delete this->root;}
