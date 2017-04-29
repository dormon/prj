#include"sdlWindow.h"

//#include<geCore/TypeRegister.h>
//#include<geCore/Accessor.h>

#include<map>
#include<iostream>

SDLWindow::SDLWindow(unsigned width,unsigned height,bool fullscreen){
  Uint32 flags = SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN;
  if(fullscreen)flags|=SDL_WINDOW_FULLSCREEN;
  this->_window = SDL_CreateWindow("sdl2",0,0,width,height,flags);
}

SDLWindow::~SDLWindow(){
  SDL_DestroyWindow(this->_window);
}

void SDLWindow::createContext(unsigned version,Profile profile,Flag flags){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,flags           );
  this->_context = std::shared_ptr<SDL_GLContext>(new SDL_GLContext,[&](SDL_GLContext*ctx){SDL_GL_DeleteContext(*ctx);delete ctx;});
  *this->_context = SDL_GL_CreateContext(this->_window);
}

void SDLWindow::setContext(SDLWindow const&other){
  this->_context = other._context;
}

void SDLWindow::makeCurrent()const{
  SDL_GL_MakeCurrent(this->_window,*this->_context);
}

void SDLWindow::swap()const{
  SDL_GL_SwapWindow(this->_window);
}

unsigned SDLWindow::getId()const{
  return SDL_GetWindowID(this->_window);
}

SDLEventProc::SDLEventProc(std::shared_ptr<ge::core::TypeRegister>const&typeRegister,bool pooling):EventProc(typeRegister,pooling){
  SDL_Init(SDL_INIT_VIDEO);
}

SDLEventProc::~SDLEventProc(){
  SDL_Quit();
}

void SDLEventProc::addWindow(std::string name,std::shared_ptr<Window>const&window){
  this->_windows[name]=window;
  this->_windowId2Name[std::dynamic_pointer_cast<SDLWindow>(window)->getId()]=name;
}

void SDLEventProc::removeWindow(std::string name){
  auto i=this->_windows.find(name);
  if(i==this->_windows.end()){
    std::cerr<<"ERROR: SDLEventProc::removeWindow - there is no window with name "<<name<<std::endl;
    return;
  }
  std::shared_ptr<SDLWindow>win=std::dynamic_pointer_cast<SDLWindow>(i->second);
  this->_windowId2Name.erase(win->getId());
  this->_windows.erase(name);
}

void SDLEventProc::operator()(){
  this->_running=true;
  SDL_Event event;
  while(this->_running){
    if(!this->_pooling)
      SDL_WaitEvent(&event);
    while(true){
      if(this->_pooling)
        if(!SDL_PollEvent(&event))break;

      auto win=this->_windowId2Name.find(event.window.windowID);
      if(win==this->_windowId2Name.end())continue;
      std::string wName=win->second;

      switch(event.type){
        case SDL_WINDOWEVENT:
          if(event.window.event==SDL_WINDOWEVENT_CLOSE){
            if(this->_windows[wName]->hasCallback("quit"))
              this->_windows[wName]->callCallback("quit");
            this->removeWindow(wName);
            if(!this->_windows.size())this->_running=false;
          }
          break;
        case SDL_QUIT:
          break;
        case SDL_KEYDOWN:
          if(this->_windows[wName]->hasCallback("keyDown")){
            ge::core::Accessor e=this->_typeRegister->allocAccessor(this->_events["keyDown"]);
            e[0]=event.key.timestamp;
            e[1]=event.key.state;
            e[2]=event.key.keysym.scancode;
            e[3]=event.key.keysym.sym;
            e[4]=event.key.keysym.mod;
            this->_windows[wName]->callCallback("keyDown",e);
          }
          break;
        case SDL_KEYUP:
          if(this->_windows[wName]->hasCallback("keyUp")){
            ge::core::Accessor e=this->_typeRegister->allocAccessor(this->_events["keyUp"]);
            e[0]=event.key.timestamp;
            e[1]=event.key.state;
            e[2]=event.key.keysym.scancode;
            e[3]=event.key.keysym.sym;
            e[4]=event.key.keysym.mod;
            this->_windows[wName]->callCallback("keyUp",e);
          }
          break;
        case SDL_MOUSEMOTION:
          if(this->_windows[wName]->hasCallback("mouseMotion")){
            ge::core::Accessor e=this->_typeRegister->allocAccessor(this->_events["mouseMotion"]);
            e[0]=event.motion.timestamp;
            e[1]=event.motion.which;
            e[2]=event.motion.state;
            e[3]=event.motion.x;
            e[4]=event.motion.y;
            e[5]=event.motion.xrel;
            e[6]=event.motion.yrel;
            this->_windows[wName]->callCallback("mouseMotion",e);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if(this->_windows[wName]->hasCallback("mouseButtonDown")){
            ge::core::Accessor e=this->_typeRegister->allocAccessor(this->_events["mouseButtonDown"]);
            e[0]=event.button.timestamp;
            e[1]=event.button.which;
            e[2]=event.button.button;
            e[3]=event.button.state;
            e[4]=event.button.clicks;
            e[5]=event.button.x;
            e[6]=event.button.y;
            this->_windows[wName]->callCallback("mouseButtonDown",e);
          }
          break;
        case SDL_MOUSEBUTTONUP:
          if(this->_windows[wName]->hasCallback("mouseButtonUp")){
            ge::core::Accessor e=this->_typeRegister->allocAccessor(this->_events["mouseButtonUp"]);
            e[0]=event.button.timestamp;
            e[1]=event.button.which;
            e[2]=event.button.button;
            e[3]=event.button.state;
            e[4]=event.button.clicks;
            e[5]=event.button.x;
            e[6]=event.button.y;
            this->_windows[wName]->callCallback("mouseButtonUp",e);
          }
          break;
        default:
          //TODO events
          break;
      }
      if(!this->_pooling)
        if(!SDL_PollEvent(&event))break;
    }
    for(auto x:this->_windows)
     if(x.second->hasCallback("idle"))
       x.second->callCallback("idle");
  }
}


/*
   std::shared_ptr<MainLoop>singletonMainLoop = nullptr;

   std::string eventTypeToStr(SDL_EventType type){
   switch(type){
   case SDL_FIRSTEVENT              :return"SDL_FIRSTEVENT";
   case SDL_QUIT                    :return"SDL_QUIT";
   case SDL_APP_TERMINATING         :return"SDL_APP_TERMINATING";
   case SDL_APP_LOWMEMORY           :return"SDL_APP_LOWMEMORY";
   case SDL_APP_WILLENTERBACKGROUND :return"SDL_APP_WILLENTERBACKGROUND";
   case SDL_APP_DIDENTERBACKGROUND  :return"SDL_APP_DIDENTERBACKGROUND";
   case SDL_APP_WILLENTERFOREGROUND :return"SDL_APP_WILLENTERFOREGROUND";
   case SDL_APP_DIDENTERFOREGROUND  :return"SDL_APP_DIDENTERFOREGROUND";
   case SDL_WINDOWEVENT             :return"SDL_WINDOWEVENT";
   case SDL_SYSWMEVENT              :return"SDL_SYSWMEVENT";
   case SDL_KEYDOWN                 :return"SDL_KEYDOWN";
   case SDL_KEYUP                   :return"SDL_KEYUP";
   case SDL_TEXTEDITING             :return"SDL_TEXTEDITING";
   case SDL_TEXTINPUT               :return"SDL_TEXTINPUT";
   case SDL_MOUSEMOTION             :return"SDL_MOUSEMOTION";
   case SDL_MOUSEBUTTONDOWN         :return"SDL_MOUSEBUTTONDOWN";
   case SDL_MOUSEBUTTONUP           :return"SDL_MOUSEBUTTONUP";
   case SDL_MOUSEWHEEL              :return"SDL_MOUSEWHEEL";
   case SDL_JOYAXISMOTION           :return"SDL_JOYAXISMOTION";
   case SDL_JOYBALLMOTION           :return"SDL_JOYBALLMOTION";
   case SDL_JOYHATMOTION            :return"SDL_JOYHATMOTION";
   case SDL_JOYBUTTONDOWN           :return"SDL_JOYBUTTONDOWN";
   case SDL_JOYBUTTONUP             :return"SDL_JOYBUTTONUP";
   case SDL_JOYDEVICEADDED          :return"SDL_JOYDEVICEADDED";
   case SDL_JOYDEVICEREMOVED        :return"SDL_JOYDEVICEREMOVED";
   case SDL_CONTROLLERAXISMOTION    :return"SDL_CONTROLLERAXISMOTION";
   case SDL_CONTROLLERBUTTONDOWN    :return"SDL_CONTROLLERBUTTONDOWN";
   case SDL_CONTROLLERBUTTONUP      :return"SDL_CONTROLLERBUTTONUP";
   case SDL_CONTROLLERDEVICEADDED   :return"SDL_CONTROLLERDEVICEADDED";
   case SDL_CONTROLLERDEVICEREMOVED :return"SDL_CONTROLLERDEVICEREMOVED";
   case SDL_CONTROLLERDEVICEREMAPPED:return"SDL_CONTROLLERDEVICEREMAPPED";
   case SDL_FINGERDOWN              :return"SDL_FINGERDOWN";
   case SDL_FINGERUP                :return"SDL_FINGERUP";
   case SDL_FINGERMOTION            :return"SDL_FINGERMOTION";
   case SDL_DOLLARGESTURE           :return"SDL_DOLLARGESTURE";
   case SDL_DOLLARRECORD            :return"SDL_DOLLARRECORD";
   case SDL_MULTIGESTURE            :return"SDL_MULTIGESTURE";
   case SDL_CLIPBOARDUPDATE         :return"SDL_CLIPBOARDUPDATE";
   case SDL_DROPFILE                :return"SDL_DROPFILE";
   case SDL_RENDER_TARGETS_RESET    :return"SDL_RENDER_TARGETS_RESET";
   case SDL_USEREVENT               :return"SDL_USEREVENT";
   case SDL_LASTEVENT               :return"SDL_LASTEVENT";
   default                          :return"";
   }
   }

   MainLoop::~MainLoop(){
   std::cout<<"MainLoop::~MainLoop()"<<std::endl;
   }

   void SDLMainLoop::operator()(){
   if(this->_running)return;
   this->_running=true;
   SDL_Event event;
   while(this->_running){
   while(SDL_PollEvent(&event)){
   auto win=this->_windowId2Name.find(event.window.windowID);
   if(win==this->_windowId2Name.end())continue;
   std::string wName=win->second;
   switch(event.type){
   case SDL_WINDOWEVENT:
   if(event.window.event==SDL_WINDOWEVENT_CLOSE){
   if(this->_windows[wName]->_quitProc)
   this->_windows[wName]->_quitProc();
   this->removeWindow(this->_windows[wName]);
   if(!this->_windows.size())this->_running=false;
}
break;
case SDL_QUIT:
break;
case SDL_KEYDOWN:
if(this->_windows[event.window.windowID]->_keyDownProc)
  this->_windows[event.window.windowID]->_keyDownProc(event);
  break;
  case SDL_KEYUP:
if(this->_windows[event.window.windowID]->_keyUpProc)
  this->_windows[event.window.windowID]->_keyUpProc(event);
  break;
  case SDL_MOUSEMOTION:
if(this->_windows[event.window.windowID]->_mouseMotionProc)
  this->_windows[event.window.windowID]->_mouseMotionProc(event);
  break;
  case SDL_MOUSEBUTTONDOWN:
if(this->_windows[event.window.windowID]->_mouseButtonDownProc)
  this->_windows[event.window.windowID]->_mouseButtonDownProc(event);
  break;
  case SDL_MOUSEBUTTONUP:
if(this->_windows[event.window.windowID]->_mouseButtonUpProc)
  this->_windows[event.window.windowID]->_mouseButtonUpProc(event);
  break;
  default:
  //TODO events
  break;
  }
}
std::cout<<eventTypeToStr((SDL_EventType)event.type)<<std::endl;
  for(auto x:this->_windows)
if(x.second->_mainLoopProc)
  x.second->_mainLoopProc();
  }
}

SDLWindow::SDLWindow(unsigned width,unsigned height,bool fullscreen){
  SDL_Init(SDL_INIT_VIDEO);
  this->_window = SDL_CreateWindow("sdl2",0,0,width,height,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  if(!singletonMainLoop){
    singletonMainLoop = std::make_shared<MainLoop>();
  }
  this->_mainLoop = singletonMainLoop;
  this->_mainLoop->addWindow(this);
}

SDLWindow::~SDLWindow(){
  std::cout<<"SDLWindow::~SDLWindow()"<<std::endl;
  this->_mainLoop->removeWindow(this);
  SDL_Quit();
}

void SDLWindow::createContext(unsigned version,Profile profile,Flag flags){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,flags           );
  this->_context = std::shared_ptr<SDL_GLContext>(new SDL_GLContext,[&](SDL_GLContext*ctx){SDL_GL_DeleteContext(*ctx);delete ctx;});
  *this->_context = SDL_GL_CreateContext(this->_window);
}

void SDLWindow::setContext(SDLWindow const&window){
  this->_context = window._context;
}

void SDLWindow::callMainLoopProc()const{
  if(this->_mainLoopProc)this->_mainLoopProc();
}

void SDLWindow::mainLoop(){
  (*this->_mainLoop)();
}

void SDLWindow::makeCurrent()const{
  SDL_GL_MakeCurrent(this->_window,*this->_context);
}


void SDLWindow::setMainLoopProc(SIMPLEPROC proc){
  this->_mainLoopProc = proc;
}

void SDLWindow::setQuitProc(SIMPLEPROC proc){
  this->_quitProc = proc;
}

void SDLWindow::swap()const{
  SDL_GL_SwapWindow(this->_window);
}

unsigned SDLWindow::getId()const{
  return SDL_GetWindowID(this->_window);
}
*/
