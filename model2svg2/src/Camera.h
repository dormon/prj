#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera{
  public:
    Camera(glm::vec3 const&pos,glm::vec3 const&tar,glm::vec3 const&up,glm::uvec2 const&ws,float fovy,float n,float f){
      view = glm::lookAt(pos,tar,up);
      projection = glm::perspectiveRH(fovy,static_cast<float>(ws.x)/static_cast<float>(ws.y),n,f);
      windowSize = ws;
      near = n;
      far = f;
    }
    glm::mat4 view;
    glm::mat4 projection;
    glm::uvec2 windowSize;
    float near;
    float far;
};

