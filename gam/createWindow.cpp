#include <Vars/Vars.h>

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

void createWindow(vars::Vars&vars){
  vars.addString("window.name","GAM");
  vars.addUint32("window.width",1024);
  vars.addUint32("window.height",768);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error(SDL_GetError());
  auto window = SDL_CreateWindow(
      vars.getString("window.name").c_str(), 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      vars.getUint32("window.width"), 
      vars.getUint32("window.height"), 
      SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
  if (!window)
    throw std::runtime_error(SDL_GetError());
  *vars.add<SDL_Window*>("window") = window;

  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
    throw std::runtime_error(SDL_GetError());
  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
    throw std::runtime_error(SDL_GetError());

  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0)
    throw std::runtime_error(SDL_GetError());
  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5) < 0)
    throw std::runtime_error(SDL_GetError());


  auto context = SDL_GL_CreateContext(window);
  if(!context)
    throw std::runtime_error(SDL_GetError());

  *vars.add<SDL_GLContext>("context") = context;

  glewExperimental = true;
  if(glewInit() != GLEW_OK)
    throw std::runtime_error("cannot initalize glew!");


  //if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP))
  //  throw std::runtime_error(SDL_GetError());
}
