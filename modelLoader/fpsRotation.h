#pragma once

#include"boxInterface.h"

class FpsRotation: public BoxInterface{
  protected:
    float _x;
    float _y;
    float _z;
    void _compute(void*);
  public:
    FpsRotation(float x,float y,float z);
};
