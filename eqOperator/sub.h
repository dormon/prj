#pragma once

#include"int.h"

class Sub: public Int{
  public:
    int _a;
    int _b;
    Sub(int a,int b);
    virtual bool operator==(Sub const&other)const;
};

