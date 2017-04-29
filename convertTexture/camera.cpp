#include"camera.h"

#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/matrix_access.hpp>


void Camera::_computeProjection(void*data){
  Camera*_this=(Camera*)data;
  _this->_projection = glm::perspective(
      glm::radians<float>(_this->_fov),
      1.f*_this->_width/_this->_height,
      _this->_near,
      _this->_far);
}

void Camera::_computeRotation(void*data){
  Camera*_this=(Camera*)data;
  _this->_rotation = (glm::rotate(glm::mat4(1.),_this->_angles[2],glm::vec3(0,0,1))*
      (glm::rotate(glm::mat4(1.),_this->_angles[0],glm::vec3(1,0,0))*
          glm::rotate(glm::mat4(1.),_this->_angles[1],glm::vec3(0,1,0))));
}

Camera::Camera(unsigned size[2],float near,float far,float fov){
  this->_width  = size[0];
  this->_height = size[1];
  this->_near   = near;
  this->_far    = far;
  this->_fov    = fov;
  this->_computeProjection(this);
  this->_computeRotation(this);
  (*this)["width" ]=pinFactory(this->_width ,true,true,this->_computeProjection,this);
  (*this)["height"]=pinFactory(this->_height,true,true,this->_computeProjection,this);
  (*this)["near"  ]=pinFactory(this->_near  ,true,true,this->_computeProjection,this);
  (*this)["far"   ]=pinFactory(this->_far   ,true,true,this->_computeProjection,this);
  (*this)["fov"   ]=pinFactory(this->_fov   ,true,true,this->_computeProjection,this);
  (*this)["position"  ]=pinFactory(this->_position,true,true);
  (*this)["projection"]=pinFactory(this->_projection,false,true);
  (*this)["rotation"  ]=pinFactory(this->_rotation,false,true);
}

Camera::~Camera(){

}

void Camera::fps(float dx,float dy,float dz){
  this->_angles[0]+=dx;
  this->_angles[1]+=dy;
  this->_angles[2]+=dz;
  this->_computeRotation(this);
}

glm::mat4 Camera::getView(){
  return this->_rotation*glm::translate(glm::mat4(1.f),-this->_position);
}

glm::mat4 Camera::getProjetion(){
  return this->_projection;
}

glm::mat4 Camera::getPV(){
  return this->getProjetion()*this->getView();
}

void Camera::forward (float x){
  this->_position-=x*glm::vec3(glm::row(this->_rotation,2));
}

void Camera::backward(float x){
  this->_position+=x*glm::vec3(glm::row(this->_rotation,2));
}

void Camera::left (float x){
  this->_position-=x*glm::vec3(glm::row(this->_rotation,0));
}

void Camera::right(float x){
  this->_position+=x*glm::vec3(glm::row(this->_rotation,0));
}

void Camera::up   (float x){
  this->_position+=x*glm::vec3(glm::row(this->_rotation,1));
}

void Camera::down (float x){
  this->_position-=x*glm::vec3(glm::row(this->_rotation,1));
}

template<>unsigned typeToId<glm::vec3>(){return 243;}
template<>std::string typeToStr<glm::vec3>(){return "glm::vec3";}

template<>unsigned typeToId<glm::mat4>(){return 244;}
template<>std::string typeToStr<glm::mat4>(){return "glm::mat4";}


