#include"window.h"

void Window::_setVersion(void*data){
  Window*_this=(Window*)data;
  if(_this->_context)SDL_GL_DeleteContext(_this->_context);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _this->_version/100       );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(_this->_version%100)/10   );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,SDL_GL_CONTEXT_DEBUG_FLAG  );
  _this->_context = SDL_GL_CreateContext(_this->_id);
}

void Window::_setSize  (void*data){
  Window*_this=(Window*)data;
  SDL_SetWindowSize(_this->_id,_this->_width,_this->_height);
}

Window::Window(unsigned width,unsigned height,unsigned version){
  this->_width   = width   ;
  this->_height  = height  ;
  this->_version = version ;
  this->_id = SDL_CreateWindow("primary",0,0,this->_width,this->_height,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  this->_setVersion(this);
  (*this)["keyDownProc"        ]=pinFactory(this->_keyDownProc        ,false,true);
  (*this)["keyUpProc"          ]=pinFactory(this->_keyUpProc          ,false,true);
  (*this)["mouseMotionProc"    ]=pinFactory(this->_mouseMotionProc    ,false,true);
  (*this)["mouseButtonDownProc"]=pinFactory(this->_mouseButtonDownProc,false,true);
  (*this)["mouseMotionUpProc"  ]=pinFactory(this->_mouseButtonUpProc  ,false,true);
  (*this)["mainLoopProc"       ]=pinFactory(this->_mainLoopProc       ,false,true);
  (*this)["quitProc"           ]=pinFactory(this->_quitProc           ,false,true);
  (*this)["version"]=pinFactory(this->_version,true,true,this->_setVersion,this);
  (*this)["width"  ]=pinFactory(this->_width ,true,true,this->_setSize,this);
  (*this)["height" ]=pinFactory(this->_height,true,true,this->_setSize,this);
}

Window::~Window(){
  SDL_GL_DeleteContext(this->_context);
  SDL_DestroyWindow(this->_id);
}

void Window::swap(){
  SDL_GL_SwapWindow(this->_id);
}

void Window::mainLoop(){
  bool running=true;
  SDL_Event event;
  while(running){
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT:
          running=false;
          break;
        case SDL_KEYDOWN:
          if(this->_keyDownProc)this->_keyDownProc(event);
          break;
        case SDL_KEYUP:
          if(this->_keyUpProc)this->_keyUpProc(event);
          break;
        case SDL_MOUSEMOTION:
          if(this->_mouseMotionProc)this->_mouseMotionProc(event);
          break;
        case SDL_MOUSEBUTTONDOWN:
          if(this->_mouseButtonDownProc)this->_mouseButtonDownProc(event);
          break;
        case SDL_MOUSEBUTTONUP:
          if(this->_mouseButtonUpProc)this->_mouseButtonUpProc(event);
          break;
          //TODO events
        default:
          break;
      }
    }
    if(this->_mainLoopProc)this->_mainLoopProc();
  }
  if(this->_quitProc)this->_quitProc();
}

template<>unsigned typeToId<Window::EVENTPROC>(){return 123;}
template<>std::string typeToStr<Window::EVENTPROC>(){return "Window::EVENTPROC";}

template<>unsigned typeToId<Window::SIMPLEPROC>(){return 124;}
template<>std::string typeToStr<Window::SIMPLEPROC>(){return "Window::SIMPLEPROC";}
