#include<iostream>
#include<stdarg.h>
#include<string>
#include<cstring>
#include<fstream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geAd/WindowObject/WindowObject.h>
#include<geGL/geGL.h>

std::shared_ptr<ge::util::WindowObject>window = nullptr;

ge::gl::BufferObject*vertices = nullptr;
ge::gl::BufferObject*indices = nullptr;
ge::gl::ProgramObject*program = nullptr;
ge::gl::VertexArrayObject*vao = nullptr;

void idle();

int main(){
  window = std::make_shared<ge::util::WindowObject>(1024,1024,false,idle,nullptr);

  glewInit();
  ge::gl::init();
  ge::gl::initShadersAndPrograms();
  ge::gl::setMediumAndGreaterDebugMessage();
  glClearColor(0,0,0,1);

  vertices = new ge::gl::BufferObject(sizeof(float)*2*4);
  float*ptr=(float*)vertices->map();
  ptr[0*2+0]=0;
  ptr[0*2+1]=0;
  ptr[1*2+0]=.5;
  ptr[1*2+1]=.5;
  ptr[2*2+0]=0;
  ptr[2*2+1]=.5;
  ptr[3*2+0]=-.5;
  ptr[3*2+1]=.3;
  vertices->unmap();

  indices = new ge::gl::BufferObject(sizeof(unsigned)*3*4);
  unsigned*iptr=(unsigned*)indices->map();
  iptr[0]=0;
  iptr[1]=0;
  iptr[2]=1;
  iptr[3]=2;
  iptr[4]=0;
  iptr[5]=1;
  iptr[6]=2;
  iptr[7]=3;
  iptr[8]=1;
  iptr[9]=2;
  iptr[10]=3;
  iptr[11]=3;
  indices->unmap();

  vao = new ge::gl::VertexArrayObject();
  vao->addAttrib(vertices->getId(),0,2,GL_FLOAT);
  vao->addElementBuffer(indices->getId());

  program = new ge::gl::ProgramObject(
      "cattmulrom.vp",
      "cattmulrom.cp",
      "cattmulrom.ep",
      "cattmulrom.fp");
  window->mainLoop();
  return 0;
}

void idle(){
  glClear(GL_COLOR_BUFFER_BIT);


  glPatchParameteri(GL_PATCH_VERTICES,4);
  vao->bind();
  program->use();
  glDrawElements(GL_PATCHES,12,GL_UNSIGNED_INT,nullptr);
  vao->unbind();

  window->swap();
}
