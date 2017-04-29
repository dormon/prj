#pragma once

#include<SDL2/SDL.h>
#include<map>
#include<memory>

class Window{
  public:
    typedef void(*EVENTPROC)(SDL_Event);
    typedef void(*SIMPLEPROC)();
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
  protected:
    SDL_Window*                   _window  = nullptr;
    std::shared_ptr<SDL_GLContext>_context = nullptr;
    std::map<unsigned,EVENTPROC>_eventProc;
    SIMPLEPROC _idleProc = nullptr;
    SIMPLEPROC _quitProc = nullptr;
  public:
    Window(unsigned width,unsigned height,bool fullscreen = false);
    ~Window();
    void createContext(unsigned version = 0u ,Profile profile = CORE,Flag flags = NONE);
    void setContext(Window const&other);
    void makeCurrent()const;
    void swap()const;
    unsigned getId()const;
    bool hasCallback(unsigned event)const;
    void setEventCallback(unsigned event,EVENTPROC proc = nullptr);
    void setQuitCallback(SIMPLEPROC proc = nullptr);
    void setIdleCallback(SIMPLEPROC proc = nullptr);
    void callCallback(unsigned event,SDL_Event data);
    bool hasQuitCallback()const;
    bool hasIdleCallback()const;
    void callQuitCallback()const;
    void callIdleCallback()const;
};

class EventProc{
  protected:
    std::map<std::string,std::shared_ptr<Window>>_name2Window;
    std::map<unsigned,std::string>_windowId2Name;
    std::map<unsigned,std::shared_ptr<Window>>_windowId2Window;
    bool _pooling = true;
    bool _running = false;
  public:
    EventProc(bool pooling = true);
    virtual ~EventProc();
    void operator()();
    void addWindow(std::string name,std::shared_ptr<Window>const&window);
    void removeWindow(std::string name);
    std::shared_ptr<Window>const&getWindow(std::string name)const;
    void setCallback(std::string window,unsigned event,Window::EVENTPROC proc = nullptr)const;
    void setIdleCallback(std::string window,Window::SIMPLEPROC proc = nullptr)const;
    void setQuitCallback(std::string window,Window::SIMPLEPROC proc = nullptr)const;
    void makeCurrent(std::string window)const;
    void swap(std::string window)const;
    void setPooling(bool pooling = true);
};

