#ifndef _LANGOBJECTS_H_
#define _LANGOBJECTS_H_

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include"lang.h"
#include"boxlang.h"

namespace lang{
  class Ladder: public Rule{
    protected:
      std::string outputSystemName;
      float    barSize;
      float    standSize;
      unsigned division;
    public:
      Ladder(
          float barSize,
          float standSize,
          unsigned division,
          std::string systemName="system",
          std::string outputSystemName="system",
          std::string ruleName="ladder");
      lang::String*body(lang::Generator*gen);
  };

  class Stairs: public Rule{
    protected:
      std::string outputSystemName;
      unsigned num;
      float overlap;
    public:
      Stairs(
          unsigned num,
          float overlap,
          std::string systemName="system",
          std::string outputSystemName="system",
          std::string ruleName="stairs");
      lang::String*body(lang::Generator*gen);
  };
}

#endif//_LANGOBJECTS_H_
