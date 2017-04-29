#ifndef _BOXLANG_H_
#define _BOXLANG_H_

#include<iostream>
#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<geGL/geGL.h>
#include<geUtil/WindowObject.h>
#include<geUtil/ArgumentObject.h>
#include<geUtil/CameraObject.h>
#include"lang.h"
#include"langstd.h"

void boxInit(std::string shaderDir);
void boxDeinit();

class BoxSymbol: public lang::Symbol
{
  public:
    glm::vec3 c;
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
    BoxSymbol(
        glm::vec3 corner,
        glm::vec3 x,
        glm::vec3 y,
        glm::vec3 z,
        std::string name="box");
    void draw(glm::mat4 mvp);
};

class SystemToBox: public lang::Rule{
  protected:
    glm::vec3 size;
    std::string boxName;
  public:
    SystemToBox(
        glm::vec3 size,
        std::string systemName = "system"     ,
        std::string boxName    = "box"        ,
        std::string ruleName   = "systemToBox");
    lang::String*body(lang::Generator*gen);
};


#endif//_BOXLANG_H_
