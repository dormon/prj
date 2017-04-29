#ifndef _COLLISION_H_
#define _COLLISION_H_

#include<vector>
#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>
#include"boxlang.h"


namespace coll{
  class OctTree{
    public:
      std::vector<BoxSymbol*>boxes;
      OctTree*child[8];
  };

  class Collision
  {
    public:
      std::vector<BoxSymbol*>boxes;
      float collision(glm::vec3&n,BoxSymbol*box,glm::vec3 a,glm::vec3 b);
      float collision(glm::vec3&n,glm::vec3 a,glm::vec3 b);
      float sphereQuadPlane(glm::vec3 c,glm::vec3 x,glm::vec3 y,glm::vec3 n,glm::vec3 a,glm::vec3 b,float r);
      float sphereSegment(
          glm::vec3 x,
          glm::vec3 y,
          glm::vec3 a,
          glm::vec3 b,
          float r);
      float spherePoint(
          glm::vec3 x,
          glm::vec3 a,
          glm::vec3 b,
          float r);
      float collision(glm::vec3&n,BoxSymbol*box,glm::vec3 a,glm::vec3 b,float r);
      float collision(glm::vec3&n,glm::vec3 a,glm::vec3 b,float r);


  };
}

#endif//_COLLISION_H_
