#pragma once

#include"int.h"

class Int;

class Mul: public Int{
  public:
    int _a;
    int _b;
    Mul(int a,int b);
    virtual bool operator==(Mul const&other)const;
};


