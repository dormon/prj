#pragma once

#include<memory>
#include<imguiSDL2Dormon/imguisdl2dormon_export.h>

struct SDL_Window;
typedef union SDL_Event SDL_Event;

namespace imguiSDL2Dormon{
  class EventHandlerImpl;
  class EventHandler;
}

class IMGUISDL2DORMON_EXPORT imguiSDL2Dormon::EventHandler{
  public:
    EventHandler(SDL_Window*window,void*sdl_gl_context = nullptr);
    ~EventHandler();
    void newFrame(SDL_Window*window);
    bool processEvent(SDL_Event* event);
  protected:
    friend class EventHandlerImpl;
    std::unique_ptr<EventHandlerImpl>impl;
};

