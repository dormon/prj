#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<SDL2/SDL.h>
#include<GL/glew.h>
#include"boxInterface.h"
#include"window.h"
#include<geGL/geGL.h>
#include"Scene.h"
#include"AssimpLoader.h"
#include"renderModel.h"

Window*window = NULL;
ge::db::ModelLoaderManager*modelLoaderManager = NULL;
ge::db::AssimpLoader*assimpLoader = NULL;
std::shared_ptr<ge::db::Scene>scene = nullptr;
std::shared_ptr<AdvancedRenderScene>rScene = nullptr;
std::shared_ptr<ge::gl::ProgramObject> drawRScene = nullptr;
std::shared_ptr<ge::gl::VertexArrayObject> emptyVAO = nullptr;
std::string shaderDir="shaders/";
std::shared_ptr<ge::gl::TextureObject>tex;

void init();
void idle();
void destroy();

int main(){
  window=new Window(1024,1024,450);
  (*window)["mainLoopProc"]->set(&idle);
  (*window)["quitProc"]->set(&destroy);
  init();
  window->mainLoop();
  delete window;
  return 0;
}

void init(){
  glewExperimental=GL_TRUE;
  glewInit();
  ge::gl::initShadersAndPrograms();

  modelLoaderManager = new ge::db::ModelLoaderManager();
  assimpLoader = new ge::db::AssimpLoader();
  modelLoaderManager->registerLoader(assimpLoader);
  scene = modelLoaderManager->load("/home/dormon/git/gshadow/models/o/o.3ds");
  rScene = std::make_shared<AdvancedRenderScene>(scene);

  emptyVAO = std::make_shared<ge::gl::VertexArrayObject>();

  tex=std::make_shared<ge::gl::TextureObject>(GL_TEXTURE_2D,GL_RGBA8,1,1,1);
  unsigned data=0xffffffff;

  //glPixelStorei(GL_UNPACK_ROW_LENGTH,1);
  glTextureSubImage2D(tex->getId(),0,0,0,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&data);
  //glPixelStorei(GL_UNPACK_ROW_LENGTH,0);

  drawRScene = std::make_shared<ge::gl::ProgramObject>(
      shaderDir+"drawRScene.vp",
      shaderDir+"drawRScene.gp",
      shaderDir+"drawRScene.fp");
}

void idle(){
  glClearColor(0,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  emptyVAO->bind();
  drawRScene->use();
  tex->bind(0);
  glDrawArrays(GL_POINTS,0,1);
  emptyVAO->unbind();
  //rScene->draw();
  window->swap();
}

void destroy(){
  delete assimpLoader;
  delete modelLoaderManager;
}

