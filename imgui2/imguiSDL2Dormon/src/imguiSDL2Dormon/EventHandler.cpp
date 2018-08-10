#include <imguiDormon/imgui.h>
#include <imguiSDL2Dormon/EventHandler.h>
#include <imguiSDL2Dormon/EventHandlerImpl.h>

namespace imguiSDL2Dormon{

EventHandler::EventHandler(SDL_Window*window,void*sdl_gl_context){
  impl = std::make_unique<EventHandlerImpl>(window,sdl_gl_context);
}

EventHandler::~EventHandler(){
}

void EventHandler::newFrame(SDL_Window*window){
  impl->newFrame(window);
}

bool EventHandler::processEvent(SDL_Event* event){
  return impl->processEvent(event);
}

}
