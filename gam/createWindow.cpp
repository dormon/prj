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

  /*
  std::cerr << "info"<< std::endl;
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  if(SDL_GetWindowWMInfo(window,&info)) {
    const char *subsystem = "an unknown system!";
    switch(info.subsystem) {
      case SDL_SYSWM_UNKNOWN:   break;
      case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows(TM)";  break;
      case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
      case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
      case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
      case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
      case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
      case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
      case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
      case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
      case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
#endif
    }
    std::cerr << subsystem << std::endl;
  }else{
    std::cerr << SDL_GetError() << std::endl;
  }
  */




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
