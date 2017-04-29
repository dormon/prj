#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstdlib>
#include<cstring>
#include<geGL/geGL.h>
#include<geGL/OpenGLCommands.h>

class Node: public std::set<std::weak_ptr<Node>>{
  public:
    virtual ~Node(){}
};

class GeGLNode: public Node{
  public:
    std::shared_ptr<ge::core::SharedCommandList>commands;
    virtual ~GeGLNode(){}
};

void splatRec(
    std::shared_ptr<ge::core::SharedCommandList>&list,
    std::shared_ptr<GeGLNode                   >node){
  for(auto x:*node->commands)
    list->push_back(x);
  for(auto n:*node)
    splatRec(list,std::dynamic_pointer_cast<GeGLNode>(n.lock()));
}

std::shared_ptr<ge::core::SharedCommandList>splat(std::shared_ptr<GeGLNode>root){
  std::shared_ptr<ge::core::SharedCommandList>result = std::make_shared<ge::core::SharedCommandList>();
  splatRec(result,root);
  return result;
}

bool hasUniform(GLuint program,GLint location){
  GLint len=0;
  GLint num=0;
  GLchar*buffer=new GLchar[len];
  glGetProgramInterfaceiv(program,GL_UNIFORM,GL_MAX_NAME_LENGTH,&len);
  glGetProgramInterfaceiv(program,GL_UNIFORM,GL_MAX_NUM_ACTIVE_VARIABLES,&num);
  for(GLint i=0;i<len;++i){
    glGetProgramResourceName(program,GL_UNIFORM,i,len,nullptr,buffer);
    if(glGetProgramResourceLocation(program,GL_UNIFORM,buffer)==location)
      return false;
  }
  return true;
}

std::shared_ptr<ge::core::SharedCommandList>removeUselesUniforms(std::shared_ptr<ge::core::SharedCommandList>&list){
  GLuint activeProgram=0;
  std::shared_ptr<ge::core::SharedCommandList>newList;
  for(unsigned i=0;i<list->size();++i){
    auto e=(*list)[i];
    if      (auto c=std::dynamic_pointer_cast<ge::gl::UseProgram<GLuint>>(e)){
      activeProgram=c->program;
      newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform1f<GLuint,GLfloat>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform1i<GLuint,GLint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform1ui<GLuint,GLuint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform1d<GLuint,GLdouble>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform2f<GLuint,GLfloat,GLfloat>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform2i<GLuint,GLint,GLint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform2ui<GLuint,GLuint,GLuint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform2d<GLuint,GLdouble,GLdouble>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform3f<GLuint,GLfloat,GLfloat,GLfloat>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform3i<GLuint,GLint,GLint,GLint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform3ui<GLuint,GLuint,GLuint,GLuint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform3d<GLuint,GLdouble,GLdouble,GLdouble>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform4f<GLuint,GLfloat,GLfloat,GLfloat,GLfloat>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform4i<GLuint,GLint,GLint,GLint,GLint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform4ui<GLuint,GLuint,GLuint,GLuint,GLuint>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }else if(auto c=std::dynamic_pointer_cast<ge::gl::Uniform4d<GLuint,GLdouble,GLdouble,GLdouble,GLdouble>>(e)){
      if(hasUniform(activeProgram,c->location))
        newList->push_back(c);
    }
  }
  return newList;
}


int main(){
  return 0;
}
