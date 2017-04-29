#pragma once

#include<iostream>
#include<GL/glew.h>

GLuint createShader(GLuint type,std::string source);

template<typename...>
void attachShaders(GLuint){}

template<typename... ARGS>
void attachShaders(GLuint program,GLuint shader,ARGS...args){
  glAttachShader(program,shader);
  attachShaders(program,args...);
}

template<typename... ARGS>
GLuint createProgram(ARGS...shaders){
  GLuint id = glCreateProgram();
  attachShaders(id,shaders...);
  glLinkProgram(id);
  glValidateProgram(id);
  GLboolean linkStatus;
  glGetProgramiv(id,GL_LINK_STATUS,(GLint*)&linkStatus);
  if(!linkStatus){
    GLuint length;
    glGetProgramiv(id,GL_INFO_LOG_LENGTH,(GLint*)&length);
    if(length){
      std::string info(length,' ');
      glGetProgramInfoLog(id,length,NULL,(GLchar*)info.c_str());//ziskani logu
      std::cerr<<info<<std::endl;
    }
    glDeleteProgram(id);
    id=0;
  }
  return id;
}

