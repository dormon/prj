#pragma once

#include<iostream>
#include<vector>
#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


namespace lang2{
  class Data{
    public:
      std::string name;
  };
  class Struct: public Data{
    public:
      std::vector<Data*>elements;
  };
  class Symbol{
    protected:
      std::string _name;
    public:
      Symbol(std::string name);
      virtual ~Symbol();
  };
}
