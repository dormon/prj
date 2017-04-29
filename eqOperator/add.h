#pragma once

#include"int.h"

class Int;

class Add: public Int{
  public:
    int _a;
    int _b;
    Add(int a,int b);
    virtual bool operator==(Add const&other)const;
};


