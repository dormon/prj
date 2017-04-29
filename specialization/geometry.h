#pragma once

#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<glm/glm.hpp>

template<unsigned dim,typename T>
class Uniform{
  protected:
  public:
    T _data[dim];
    void operator()();
};

template<>void Uniform<1,float>::operator()();
template<>void Uniform<2,float>::operator()();

