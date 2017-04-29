#include"shaderObjectFactory.h"

#include<geGL/geGL.h>

BEGIN_INTERPRET_FUNCTION_CPP(ShaderObjectFactory,"SharedShader","sharedShader","string","source","u32","type");
MID_INTERPRET_FUNCTION_CPP(ShaderObjectFactory);
  if(!this->hasInput("source")||!this->hasInput("type")||!this->hasOutput())return false;
  if(!this->_inputChanged("type")&&!this->_inputChanged("source"))return false;
  (std::shared_ptr<ge::gl::ShaderObject>&)*this->_getOutput().data = std::make_shared<ge::gl::ShaderObject>(
      (GLenum)*this->getInputData("type"),(std::string&)*this->getInputData("source"));
  return true;
END_INTERPRET_FUNCTION_CPP();

