#pragma once

#include<SDL2/SDL.h>
#include<memory>
#include<vector>
#include<set>
#include<map>

#include"window.h"

class SDLWindow: public Window{
  protected:
    std::shared_ptr<SDL_GLContext>_context = nullptr;
    SDL_Window*_window = nullptr;
  public:
    enum Profile{
      CORE          = SDL_GL_CONTEXT_PROFILE_CORE         ,
      COMPATIBILITY = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
      ES            = SDL_GL_CONTEXT_PROFILE_ES           ,
    };
    enum Flag{
      NONE               = 0                                     ,
      DEBUG              = SDL_GL_CONTEXT_DEBUG_FLAG             ,
      FORWARD_COMPATIBLE = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
      ROBUST_ACCESS      = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG     ,
      RESET_ISOLATION    = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG   ,
    };
    SDLWindow(unsigned width,unsigned height,bool fullscreen = false);
    virtual ~SDLWindow();
    void createContext(unsigned version = 450u ,Profile profile = CORE,Flag flags = NONE);
    void setContext(SDLWindow const&other);
    virtual void makeCurrent()const;
    virtual void swap()const;
    unsigned getId()const;
};

class SDLEventProc: public EventProc{
  protected:
    std::map<unsigned,std::string>_windowId2Name;
  public:
    SDLEventProc(std::shared_ptr<ge::core::TypeRegister>const&typeRegister,bool pooling = true);
    virtual void addWindow(std::string name,std::shared_ptr<Window>const&window);
    virtual void removeWindow(std::string name);
    virtual ~SDLEventProc();
    virtual void operator()();
};


