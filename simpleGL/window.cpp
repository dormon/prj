#include"window.h"
#include<iostream>

Window::Window(unsigned width,unsigned height,bool fullscreen){
  Uint32 flags = SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN;
  if(fullscreen)flags|=SDL_WINDOW_FULLSCREEN;
  this->_window = SDL_CreateWindow("sdl2",0,0,width,height,flags);
}

Window::~Window(){
  SDL_DestroyWindow(this->_window);
}

void Window::createContext(unsigned version,Profile profile,Flag flags){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,flags           );
  SDL_GLContext ctx=nullptr;
  if(version!=0){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
    ctx = SDL_GL_CreateContext(this->_window);
    if(!ctx)
      std::cerr<<SDL_GetError()<<std::endl;
  }else{
    const unsigned versions[]={450,440,430,420,410,400,330,320,310,210,200};
    for(unsigned i=0;i<sizeof(versions)/sizeof(unsigned);++i){
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versions[i]/100    );
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(versions[i]%100)/10);
      ctx = SDL_GL_CreateContext(this->_window);
      if(ctx)break;
    }
  }
  if(!ctx)return;
  this->_context = std::shared_ptr<SDL_GLContext>(new SDL_GLContext,[&](SDL_GLContext*ctx){SDL_GL_DeleteContext(*ctx);delete ctx;});
  *this->_context=ctx;
}

void Window::setContext(Window const&other){
  this->_context = other._context;
}

void Window::makeCurrent()const{
  SDL_GL_MakeCurrent(this->_window,*this->_context);
}

void Window::swap()const{
  SDL_GL_SwapWindow(this->_window);
}

unsigned Window::getId()const{
  return SDL_GetWindowID(this->_window);
}

bool Window::hasCallback(unsigned event)const{
  return this->_eventProc.find(event)!=this->_eventProc.end();
}

void Window::setEventCallback(unsigned event,EVENTPROC proc){
  this->_eventProc[event]=proc;
}

void Window::setQuitCallback(SIMPLEPROC proc){
  this->_quitProc = proc;
}

void Window::setIdleCallback(SIMPLEPROC proc){
  this->_idleProc = proc;
}

void Window::callCallback(unsigned event,SDL_Event data){
  auto ii=this->_eventProc.find(event);
  if(ii==this->_eventProc.end())return;
  ii->second(data);
}

bool Window::hasQuitCallback()const{
  return this->_quitProc!=nullptr;
}

bool Window::hasIdleCallback()const{
  return this->_idleProc!=nullptr;
}

void Window::callQuitCallback()const{
  if(this->_quitProc)this->_quitProc();
}
void Window::callIdleCallback()const{
  if(this->_idleProc)this->_idleProc();
}

EventProc::EventProc(bool pooling){
  this->_pooling = pooling;
  SDL_Init(SDL_INIT_VIDEO);
}

EventProc::~EventProc(){
  this->_name2Window.clear();
  this->_windowId2Window.clear();
  this->_windowId2Window.clear();
  SDL_Quit();
}

void EventProc::addWindow(std::string name,std::shared_ptr<Window>const&window){
  this->_name2Window[name]=window;
  this->_windowId2Window[window->getId()]=window;
  this->_windowId2Name[window->getId()]=name;
}

void EventProc::removeWindow(std::string name){
  this->_windowId2Name.erase(this->_name2Window[name]->getId());
  this->_windowId2Window.erase(this->_name2Window[name]->getId());
  this->_name2Window.erase(name);
}

void EventProc::operator()(){
  this->_running=true;
  SDL_Event event;
  while(this->_running){
    if(!this->_pooling)
      SDL_WaitEvent(&event);
    while(true){
      if(this->_pooling)
        if(!SDL_PollEvent(&event))break;

      auto win=this->_windowId2Window.find(event.window.windowID);
      if(win==this->_windowId2Window.end())continue;
      auto w=win->second;

      switch(event.type){
        case SDL_WINDOWEVENT:
          if(event.window.event==SDL_WINDOWEVENT_CLOSE){
            if(w->hasQuitCallback())
              w->callQuitCallback();
            this->removeWindow(this->_windowId2Name[w->getId()]);
            if(!this->_windowId2Window.size())this->_running=false;
          }
          break;
        case SDL_QUIT:
          break;
        default:
          w->callCallback(event.type,event);
          break;
      }
      if(!this->_pooling)
        if(!SDL_PollEvent(&event))break;
    }
    for(auto x:this->_windowId2Window)
      if(x.second->hasIdleCallback())
        x.second->callIdleCallback();
  }
}

std::shared_ptr<Window>const&EventProc::getWindow(std::string name)const{
  auto i=this->_name2Window.find(name);
  if(i==this->_name2Window.end()){
    std::cerr<<"ERROR: there is no window with name "<<name<<std::endl;
    static std::shared_ptr<Window> er;
    return er;
  }
  return i->second;
}


void EventProc::setCallback(std::string window,unsigned event,Window::EVENTPROC proc)const{
  this->getWindow(window)->setEventCallback(event,proc);
}

void EventProc::setIdleCallback(std::string window,Window::SIMPLEPROC proc)const{
  this->getWindow(window)->setIdleCallback(proc);
}

void EventProc::setQuitCallback(std::string window,Window::SIMPLEPROC proc)const{
  this->getWindow(window)->setQuitCallback(proc);
}

void EventProc::makeCurrent(std::string window)const{
  this->getWindow(window)->makeCurrent();
}

void EventProc::swap(std::string window)const{
  this->getWindow(window)->swap();
}

void EventProc::setPooling(bool pooling){
  this->_pooling = pooling;
}
