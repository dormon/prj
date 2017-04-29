#include"langstd.h"

using namespace lang;

SystemSymbol::SystemSymbol(
    glm::vec3 corner,
    glm::vec3 x,
    glm::vec3 y,
    glm::vec3 z,
    std::string name):lang::Symbol(name){
  this->corner=corner;
  this->x=x;
  this->y=y;
  this->z=z;
}

GridXY::GridXY(
    glm::vec2 coord,
    glm::uvec2 division,
    std::string systemName,
    std::string outputSystemName,
    std::string ruleName    
    ):lang::Rule(ruleName,systemName){
  this->coord=coord;
  this->division=division;
  this->outputSystemName=outputSystemName;
}


lang::String*GridXY::body(lang::Generator*gen){
  lang::String*result=new lang::String();
  SystemSymbol*parent=(SystemSymbol*)gen->currentString->string[gen->currentPosition];
  for(unsigned x=0;x<this->division.x;++x){
    for(unsigned y=0;y<this->division.y;++y){
      result->string.push_back(
          new SystemSymbol(
            parent->corner+
            parent->x*(float)x/(float)this->division.x+
            parent->y*(float)y/(float)this->division.y,
            parent->x/(float)this->division.x,
            parent->y/(float)this->division.y,
            parent->z,
            this->outputSystemName));
    }
  }
  return result;
}
CircleXY::CircleXY(
    float width,
    unsigned division,
    float startAngle,
    float angleRange,
    std::string systemName,
    std::string outputSystemName,
    std::string ruleName):lang::Rule(ruleName,systemName){
  this->width=width;
  this->division=division;
  this->outputSystemName=outputSystemName;
  this->startAngle=startAngle;
  this->angleRange=angleRange;
}

lang::String*CircleXY::body(lang::Generator*gen){
  lang::String*result=new lang::String();
  SystemSymbol*parent=(SystemSymbol*)gen->currentString->string[gen->currentPosition];
  for(unsigned x=0;x<this->division;++x){
//    float angle0  = 2.f*glm::pi<float>()*(x+0.f )/this->division;
//    float angle1  = 2.f*glm::pi<float>()*(x+1.f )/this->division;
      float angle0  = this->startAngle+this->angleRange*(x+0.f )/this->division;
      float angle1  = this->startAngle+this->angleRange*(x+1.f )/this->division;


    float cosa0   = glm::cos(angle0);
    float sina0   = glm::sin(angle0);
    float cosa1   = glm::cos(angle1);
    float sina1   = glm::sin(angle1);

    glm::vec3 center=parent->corner+parent->x/2.f+parent->y/2.f;
    glm::vec3 A=parent->x*cosa0/2.f+parent->y*sina0/2.f+center;
    glm::vec3 B=parent->x*cosa1/2.f+parent->y*sina1/2.f+center;
    float radius=glm::min(glm::length(parent->x),glm::length(parent->y))*this->width/2.f;
    glm::vec3 newY=B-A;
    glm::vec3 newZ=parent->z;
    glm::vec3 newX=glm::normalize(glm::cross(newY,newZ))*radius;

    result->string.push_back(
        new lang::SystemSymbol(
          A-newX,
          newX,
          newY,
          newZ,
          this->outputSystemName));
  }
  return result;
}

