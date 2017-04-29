#ifndef _QUADLANG_H_
#define _QUADLANG_H_

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


class QuadSymbol: public lang::Symbol
{
  public:
    glm::vec2 corner;
    glm::vec2 xAxe;
    glm::vec2 yAxe;
    QuadSymbol(glm::vec2 corner,glm::vec2 x,glm::vec2 y):lang::Symbol("quad2"){
      this->corner=corner;
      this->xAxe=x;
      this->yAxe=y;
    }
    virtual void copyData(QuadSymbol*sym){
      std::cerr<<"copy data\n";
      QuadSymbol*s=(QuadSymbol*)sym;
      this->corner=s->corner;
      this->xAxe=s->xAxe;
      this->yAxe=s->yAxe;
    }
    void draw(){
      glBegin(GL_LINE_LOOP);
      glVertex4f(this->corner.x,this->corner.y,0,1);
      glVertex4f(this->corner.x+this->xAxe.x,this->corner.y+this->xAxe.y,0,1);
      glVertex4f(this->corner.x+this->xAxe.x+this->yAxe.x,this->corner.y+this->xAxe.y+this->yAxe.y,0,1);
      glVertex4f(this->corner.x+this->yAxe.x,this->corner.y+this->yAxe.y,0,1);
      glEnd();
    }
};

class RandomQuadSplit: public lang::Rule
{
  private:
    float reducedRange;
    float probability;
  public:
    RandomQuadSplit(float reducedRange,float probability):lang::Rule("randomQuadSplit","quad2"){
      this->reducedRange=reducedRange;
      this->probability=probability;
    }
    bool condition(lang::Generator*gen){
      return gen->_random->getUniform()<this->probability;
    }
    lang::String*body(lang::Generator*gen){
      lang::String*result=new lang::String();
      QuadSymbol*parent=(QuadSymbol*)gen->currentString->string[gen->currentPosition];
      unsigned axe=gen->_random->getUniform()>glm::length(parent->xAxe)/(glm::length(parent->xAxe)+glm::length(parent->yAxe));
      float mid=gen->_random->getValue(this->reducedRange,1-this->reducedRange);
      
      if(axe==0){
        result->string.push_back(new QuadSymbol(
              parent->corner,
              parent->xAxe*mid,
              parent->yAxe));
        result->string.push_back(new QuadSymbol(
              parent->corner+parent->xAxe*mid,
              parent->xAxe*(1-mid),
              parent->yAxe));
      }else{
        result->string.push_back(new QuadSymbol(
              parent->corner,
              parent->xAxe,
              parent->yAxe*mid));
        result->string.push_back(new QuadSymbol(
              parent->corner+parent->yAxe*mid,
              parent->xAxe,
              parent->yAxe*(1-mid)));
      }
      return result;
    }
};

class QuadTo4Quad: public lang::Rule
{
  private:
    float _probability;
  public:
    QuadTo4Quad(float probability):lang::Rule("quadTo4Quad","quad2"){this->_probability=probability;}
    bool condition(lang::Generator*gen){
      return gen->_random->getUniform()<this->_probability;
    }
    lang::String*body(lang::Generator*gen){
      lang::String*result=new lang::String();
      QuadSymbol*parent=(QuadSymbol*)gen->currentString->string[gen->currentPosition];
      result->string.push_back(new QuadSymbol(
            parent->corner,
            parent->xAxe*.5f,
            parent->yAxe*.5f));
      result->string.push_back(new QuadSymbol(
            parent->corner+parent->xAxe*.5f,
            parent->xAxe*.5f,
            parent->yAxe*.5f));
      result->string.push_back(new QuadSymbol(
            parent->corner+parent->yAxe*.5f,
            parent->xAxe*.5f,
            parent->yAxe*.5f));
      result->string.push_back(new QuadSymbol(
            parent->corner+parent->xAxe*.5f+parent->yAxe*.5f,
            parent->xAxe*.5f,
            parent->yAxe*.5f));
      return result;
    }


};


#endif//_QUADLANG_H_
