#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<SDL2/SDL.h>
#include<GL/_glew.h>
#include"sdlWindow.h"

std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;
EventProc*mainLoop=nullptr;

void primIdle(){
  mainLoop->makeCurrent("prim");
  glClearColor(0,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  mainLoop->swap("prim");
}

void secIdle(){
  mainLoop->makeCurrent("sec");
  glClearColor(0,0,1,1);
  glClear(GL_COLOR_BUFFER_BIT);
  mainLoop->swap("sec");
}

void primKeyDown(ge::core::Accessor const&data){
  std::cout<<data.data2Str()<<std::endl;
}

int main(){
  typeRegister = std::make_shared<ge::core::TypeRegister>();
  mainLoop=new SDLEventProc(typeRegister);
  mainLoop->addWindow("prim",std::make_shared<SDLWindow>(1000,1000));
  mainLoop->addWindow("sec",std::make_shared<SDLWindow>(100,100));
  std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("prim"))->createContext();
  std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("sec"))->setContext(*std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("prim")));
  mainLoop->setCallback("prim","idle"   ,primIdle   );
  mainLoop->setCallback("prim","keyDown",primKeyDown);
  mainLoop->setCallback("sec" ,"idle"   ,secIdle    );
  (*mainLoop)();
  delete mainLoop;
  return 0;
}
