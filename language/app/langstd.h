#ifndef _LANGSTD_H_
#define _LANGSTD_H_

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/constants.hpp>
#include<glm/gtc/matrix_access.hpp>

#include"lang.h"

namespace lang{
  class SystemSymbol: public Symbol{
    public:
      glm::vec3 corner;
      glm::vec3 x;
      glm::vec3 y;
      glm::vec3 z;
      SystemSymbol(glm::vec3 corner,glm::vec3 x,glm::vec3 y,glm::vec3 z,std::string name);
  };

  class GridXY: public Rule{
    protected:
      glm::vec2 coord;
      glm::uvec2 division;
      std::string outputSystemName;
    public:
      GridXY(
          glm::vec2 coord,
          glm::uvec2 division,
          std::string systemName="system",
          std::string outputSystemName="system",
          std::string ruleName="gridXY");
      lang::String*body(lang::Generator*gen);
  };

  class CircleXY: public Rule{
    protected:
      float startAngle;
      float angleRange;
      float width;
      unsigned division;
      std::string outputSystemName;
    public:
      CircleXY(
          float width,
          unsigned division,
          float startAngle=0,
          float angleRange=2*glm::pi<float>(),
          std::string systemName="system",
          std::string outputSystemName="system",
          std::string ruleName="circleXY");
      lang::String*body(lang::Generator*gen);
  };
}



#endif//_LANGSTD_H_
