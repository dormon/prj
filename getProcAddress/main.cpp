#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<glm/glm.hpp>
#include<algorithm>

#include <dlfcn.h>
#include<geCore/ErrorPrinter.h>
#include<geAd/SDLWindow/SDLWindow.h>
#include<geAd/SDLWindow/SDLMainLoop.h>
#include<geGL/geGL.h>
class LoaderOpenGLFunction{
  protected:
    bool _triedToLoadOpenGL = false;
    bool _triedToLoadGetProcAddress = false;
    void*openglLib = nullptr;
    using PROC = void(*)();
    using GETPROCTYPE = PROC(*)(uint8_t const*);
    GETPROCTYPE _glXGetProcAddress = nullptr;
  public:
    void*operator()(char const*name){
      if(!this->_triedToLoadOpenGL){
        this->_triedToLoadOpenGL = true;
        this->openglLib = dlopen("libGL.so.1",RTLD_LAZY);
      }
      if(!this->_triedToLoadGetProcAddress){
        this->_triedToLoadGetProcAddress = true;
        if(this->openglLib)
          reinterpret_cast<void*&>(this->_glXGetProcAddress) = dlsym(this->openglLib,"glXGetProcAddress");
        else ge::core::printError(GE_CORE_FCENAME,"cannot open libGL.so.1");
      }
      if(!this->_glXGetProcAddress){
        ge::core::printError(GE_CORE_FCENAME,"cannot load glXGetProcAddress");
        return nullptr;
      }
      return (void*)this->_glXGetProcAddress((uint8_t const*)(name));
    }
    ~LoaderOpenGLFunction(){
      std::cerr<<"LoaderOpenGLFunction::~LoaderOpenGLFunction()"<<std::endl;
      if(openglLib)dlclose(this->openglLib);
      this->openglLib = nullptr;
    }
};

LoaderOpenGLFunction loader{};
void*getAdr(char const*name){
  return loader(name);
}



int main(){
  auto mainLoop = std::make_shared<ge::ad::SDLMainLoop>();
  auto window = std::make_shared<ge::ad::SDLWindow>();

  window->createContext("rendering");
  mainLoop->addWindow("mainWindow", window);

  ge::gl::init();
  std::cout<<getAdr               ("glGetProgramInterfaceiv")<<std::endl;
  std::cout<<loader               ("glGetProgramInterfaceiv")<<std::endl;
  std::cout<<SDL_GL_GetProcAddress("glGetProgramInterfaceiv")<<std::endl;
  return 0;
}
