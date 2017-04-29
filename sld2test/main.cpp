#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cassert>
#include<SDL2/SDL.h>
#include<GL/glew.h>

int main(){
  uint32_t width = 1024;
  uint32_t height = 768;
  uint32_t windowFlags = SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN;
  uint32_t version = 450;
  uint32_t profile = SDL_GL_CONTEXT_PROFILE_CORE;
  uint32_t flags = SDL_GL_CONTEXT_DEBUG_FLAG;

  auto window = SDL_CreateWindow("",0,0,width,height,windowFlags);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,flags           );
  auto ctx = SDL_GL_CreateContext(window);

  glewExperimental = GL_TRUE;
  glewInit();

  bool running = true;
  SDL_Event event;
  while(running){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_WINDOWEVENT){
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
          running = false;
      }
    }
    glClearColor(0.f,1.f,0.f,0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
  return 0;
}





