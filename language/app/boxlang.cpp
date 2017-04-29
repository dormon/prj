#include"boxlang.h"

using namespace lang;

ge::gl::ProgramObject*drawCube=NULL;
void boxInit(std::string shaderDir){
  drawCube = new  ge::gl::ProgramObject(
      shaderDir+"drawobb.vp",
      shaderDir+"drawobb.gp",
      shaderDir+"drawobb.fp");

}
void boxDeInit(){
  delete drawCube;
}

BoxSymbol::BoxSymbol(
    glm::vec3 corner,
    glm::vec3 x,
    glm::vec3 y,
    glm::vec3 z,
    std::string name):lang::Symbol(name)
{
  this->c=corner;
  this->x=x;
  this->y=y;
  this->z=z;
}
void BoxSymbol::draw(glm::mat4 mvp){
  drawCube->use();
  drawCube->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  drawCube->set("corner",1,glm::value_ptr(this->c));
  drawCube->set("xAxis" ,1,glm::value_ptr(this->x));
  drawCube->set("yAxis" ,1,glm::value_ptr(this->y));
  drawCube->set("zAxis" ,1,glm::value_ptr(this->z));
  glDrawArrays(GL_POINTS,0,1);
}


SystemToBox::SystemToBox(
    glm::vec3 size,
    std::string systemName,
    std::string boxName,
    std::string ruleName
    ):Rule(ruleName,systemName){
  this->boxName=boxName;
  this->size=size;
}

lang::String*SystemToBox::body(lang::Generator*gen){
  lang::String*result=new lang::String();
  SystemSymbol*parent=(SystemSymbol*)gen->currentString->string[gen->currentPosition];
  result->string.push_back(
      new BoxSymbol(
        parent->corner,
        parent->x*this->size.x,
        parent->y*this->size.y,
        parent->z*this->size.z,
        this->boxName)
      );
  return result;
}

