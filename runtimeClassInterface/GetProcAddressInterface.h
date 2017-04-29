#pragma once

class GetProcAddress{
  public:
    virtual~GetProcAddress(){}
    virtual void init(){}
    virtual void*get(const char*)=0;
};
