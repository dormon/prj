#pragma once

extern "C"{
class Loader{
  public:
    virtual ~Loader(){};
    virtual void print()=0;
};
}
