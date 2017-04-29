#pragma once

#include<glm/glm.hpp>
#include"boxInterface.h"

class Camera: public BoxInterface{
  protected:
    glm::mat4 _rotation  ;
    glm::mat4 _projection;
    glm::vec3 _position  ;
    unsigned _width ;
    unsigned _height;
    float    _near;
    float    _far;
    float    _fov;
    float    _angles[3]={0,0,0};
    static void _computeProjection(void*);
    static void _computeRotation(void*);
  public:
    Camera(unsigned size[2],float near,float far,float fov);
    virtual ~Camera();
    void fps(float dx,float dy,float dz);
    glm::mat4 getView();
    glm::mat4 getProjetion();
    glm::mat4 getPV();
    void forward (float x);
    void backward(float x);
    void left (float x);
    void right(float x);
    void up   (float x);
    void down (float x);
};

template<>unsigned typeToId<glm::vec3>();
template<>std::string typeToStr<glm::vec3>();

template<>unsigned typeToId<glm::mat4>();
template<>std::string typeToStr<glm::mat4>();
