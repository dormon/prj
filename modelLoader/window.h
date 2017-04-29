#pragma once

#include<SDL2/SDL.h>
#include"boxInterface.h"


class Window: public BoxInterface{
  public:
    typedef void(*EVENTPROC)(SDL_Event);
    typedef void(*SIMPLEPROC)();
  protected:
    SDL_Window*    _id              = nullptr;
    SDL_GLContext  _context         = nullptr;
    EVENTPROC  _keyDownProc         = nullptr;
    EVENTPROC  _keyUpProc           = nullptr;
    EVENTPROC  _mouseMotionProc     = nullptr;
    EVENTPROC  _mouseButtonDownProc = nullptr;
    EVENTPROC  _mouseButtonUpProc   = nullptr;
    SIMPLEPROC _mainLoopProc        = nullptr;
    SIMPLEPROC _quitProc            = nullptr;
    static void _setVersion(void*);
    static void _setSize   (void*);
    unsigned _width      = 640  ;
    unsigned _height     = 480  ;
    unsigned _version    = 450  ;
    bool     _fullscreen = false;
  public:
    Window(unsigned width = 640u,unsigned height = 480u,unsigned version = 450);
    ~Window();
    void swap();
    void mainLoop();
};

template<>unsigned typeToId<Window::EVENTPROC>();
template<>std::string typeToStr<Window::EVENTPROC>();

template<>unsigned typeToId<Window::SIMPLEPROC>();
template<>std::string typeToStr<Window::SIMPLEPROC>();
