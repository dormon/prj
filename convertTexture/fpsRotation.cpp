#include"fpsRotation.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

void FpsRotation::_compute(void*data){
  FpsRotation*_this=(FpsRotation*)data;
  (*_this)["rotation"]->set(
    (glm::rotate(glm::mat4(1.),_this->_z,glm::vec3(0,0,1))*
      (glm::rotate(glm::mat4(1.),_this->_x,glm::vec3(1,0,0))*
          glm::rotate(glm::mat4(1.),_this->_y,glm::vec3(0,1,0)))));
}

FpsRotation::FpsRotation(float x,float y,float z){
  this->_x=x;
  this->_y=y;
  this->_z=z;
  /*
  (*this)["x"]=pinFactory(this->_x,true,true,this->_compute,this);
  (*this)["y"]=pinFactory(this->_y,true,true,this->_compute,this);
  (*this)["z"]=pinFactory(this->_z,true,true,this->_compute,this);
  (*this)["rotation"]=pinFactory<glm::mat4>(true,false);
  */
}
