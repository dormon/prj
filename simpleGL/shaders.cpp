#include"shaders.h"

GLint getShaderParam(GLuint id,GLenum pname){
  GLint params;
  glGetShaderiv(id,pname,&params);//ziskani parametru shaderu
  return params;
}

GLuint createShader(GLuint type,std::string source){
  GLuint id = glCreateShader(type);//rezervace id shaderu
  const char*string=source.c_str();
  glShaderSource(id,1,&string,NULL);//pripojeni zdrojaku k shaderu
  glCompileShader(id);//kompilace zdrojaku
  GLboolean compileStatus = getShaderParam(id,GL_COMPILE_STATUS);
  if(!compileStatus){
    GLuint length = getShaderParam(id,GL_INFO_LOG_LENGTH);
    if(length){
      std::string info(length,' ');
      glGetShaderInfoLog(id,length,NULL,(GLchar*)info.c_str());//ziskani logu
      std::cerr<<info<<std::endl;
    }
    glDeleteShader(id);//uvolneni id shaderu
    id = 0;
  }
  return id;
}

GLint getProgramParam(GLuint id,GLenum pname){
  GLint params;
  glGetProgramiv(id,pname,&params);
  return params;
}
