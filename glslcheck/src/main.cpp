#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<geGL/geGL.h>

#include<iostream>

int main(int argc,char*argv[]){
  unsigned          version     = 440                        ;
  SDL_GLprofile     profile     = SDL_GL_CONTEXT_PROFILE_CORE;
  SDL_GLcontextFlag contextFlag = SDL_GL_CONTEXT_DEBUG_FLAG  ;

  SDL_Init(SDL_INIT_VIDEO);//initialise video
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,contextFlag     );

  SDL_Window*mainWindow=SDL_CreateWindow("glslchek",0,0,100,100,SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(mainWindow);
  glewExperimental=GL_TRUE;
  glewInit();

  ge::gl::initShadersAndPrograms();
  ge::gl::setHighDebugMessage();


  SDL_Quit();
  return 0;
}
