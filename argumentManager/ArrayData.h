#pragma once

#include<vector>
#include"ArgData.h"

class ArrayData: public ArgData{
  protected:
    ArgData::Type _innerType;
    std::vector<ArgData*>_data;
  public:
    ArrayData(ArrayData::Type innerType);
    virtual ~ArrayData();
    ArgData*operator[](unsigned i);
    void add(ArgData*item);
    unsigned size();
    ArgData::Type getInnerType();
    virtual bool sameType(ArgData*other);
    virtual std::string toStr();
};

