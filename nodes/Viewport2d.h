#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


#include<Viewport.h>

class Viewport2d: public Viewport{
  public:
    glm::vec2 size;
    glm::vec2 cameraPosition = glm::vec2(0.f);
    float cameraScale = 1.f;
    float cameraAngle = 0.f;
    Viewport2d(glm::vec2 const&s = glm::vec2(0.f));
    virtual~Viewport2d();
};

inline Viewport2d::Viewport2d(glm::vec2 const&s):Viewport(),size(s){}

inline Viewport2d::~Viewport2d(){
  ___;
}

