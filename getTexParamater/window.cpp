#include"window.h"

#include<iostream>

Window::Window(){
}

Window::~Window(){
}

bool Window::hasCallback(std::string event)const{
  auto i=this->_eventProc.find(event);
  if(i!=this->_eventProc.end())
    return i->second!=nullptr;
  auto j=this->_simpleProc.find(event);
  if(j!=this->_simpleProc.end())
    return j->second!=nullptr;
  return false;
}

void Window::setCallback(std::string event,EVENTPROC  proc){
  this->_eventProc[event]=proc;
}

void Window::setCallback(std::string event,SIMPLEPROC proc){
  this->_simpleProc[event]=proc;
}

void Window::callCallback(std::string proc){
  auto i=this->_simpleProc.find(proc);
  if(i==this->_simpleProc.end())return;
  if(i->second)i->second();
}

void Window::callCallback(std::string proc,ge::core::Accessor const&data){
  auto i=this->_eventProc.find(proc);
  if(i==this->_eventProc.end())return;
  i->second(data);
}

EventProc::EventProc(std::shared_ptr<ge::core::TypeRegister>const&typeRegister,bool pooling){
  this->_typeRegister = typeRegister;
  this->_pooling = pooling;
  ge::core::TypeRegister::TypeID keyboardEvent = this->_typeRegister->addType("KeyboardEventData",
      ge::core::TypeRegister::STRUCT,
      6,
      ge::core::TypeRegister::U32,//timestamp
      ge::core::TypeRegister::U8 ,//state
      ge::core::TypeRegister::U8 ,//repeat
      ge::core::TypeRegister::U32,//scancode
      ge::core::TypeRegister::I32,//sym
      ge::core::TypeRegister::I16 //modifiers
      );
  ge::core::TypeRegister::TypeID mouseButtonEvent = this->_typeRegister->addType("MouseButtonEventData",
      ge::core::TypeRegister::STRUCT,
      7,
      ge::core::TypeRegister::U32,//timestamp
      ge::core::TypeRegister::U32,//which
      ge::core::TypeRegister::U8 ,//button
      ge::core::TypeRegister::U8 ,//state
      ge::core::TypeRegister::U8 ,//clicks
      ge::core::TypeRegister::I32,//x
      ge::core::TypeRegister::I32 //y
      );
  ge::core::TypeRegister::TypeID mouseMotionEvent = this->_typeRegister->addType("MouseMotionEventData",
      ge::core::TypeRegister::STRUCT,
      7,
      ge::core::TypeRegister::U32,//timestamp
      ge::core::TypeRegister::U32,//which
      ge::core::TypeRegister::U8 ,//state
      ge::core::TypeRegister::I32,//x
      ge::core::TypeRegister::I32,//y
      ge::core::TypeRegister::I32,//xrel
      ge::core::TypeRegister::I32 //yrel
      );
  ge::core::TypeRegister::TypeID mouseWheelEvent = this->_typeRegister->addType("MouseWheelEventData",
      ge::core::TypeRegister::STRUCT,
      4,
      ge::core::TypeRegister::U32,//timestamp
      ge::core::TypeRegister::U32,//which
      ge::core::TypeRegister::I32,//x
      ge::core::TypeRegister::I32 //y
      );

  this->registerEvent("quit",this->_typeRegister->getTypeId("void"));
  this->registerEvent("idle",this->_typeRegister->getTypeId("void"));
  this->registerEvent("keyDown"        ,keyboardEvent   );
  this->registerEvent("keyUp"          ,keyboardEvent   );
  this->registerEvent("mouseButtonDown",mouseButtonEvent);
  this->registerEvent("mouseButtonUp"  ,mouseButtonEvent);
  this->registerEvent("mouseMotion"    ,mouseMotionEvent);
  this->registerEvent("mouseWheel"     ,mouseWheelEvent );
  //TODO register basic events and events data types
}

EventProc::~EventProc(){

}

/*
void EventProc::operator()(){
  std::cerr<<"WARNING: there is no implementation of EventProc::operator()()"<<std::endl;
}

void EventProc::addWindow(std::string name,std::shared_ptr<Window>const&window){
  this->_windows[name]=window;
}

void EventProc::removeWindow(std::string name){
  this->_windows.erase(name);
}
*/

std::shared_ptr<Window>const&EventProc::getWindow(std::string name)const{
  auto i=this->_windows.find(name);
  if(i==this->_windows.end()){
    std::cerr<<"ERROR: there is no window with name "<<name<<std::endl;
    static std::shared_ptr<Window> er;
    return er;
  }
  return i->second;
}

void EventProc::setCallback(std::string window,std::string event,Window::SIMPLEPROC proc)const{
  this->getWindow(window)->setCallback(event,proc);
}

void EventProc::setCallback(std::string window,std::string event,Window::EVENTPROC proc)const{
  this->getWindow(window)->setCallback(event,proc);
}

void EventProc::makeCurrent(std::string window)const{
  this->getWindow(window)->makeCurrent();
}

void EventProc::swap(std::string window)const{
  this->getWindow(window)->swap();
}

void EventProc::registerEvent(std::string name,ge::core::TypeRegister::TypeID dataType){
  this->_events[name]=dataType;
}

EventProc::EventIterator EventProc::eventBegin()const{
  return this->_events.begin();
}

EventProc::EventIterator EventProc::eventEnd()const{
  return this->_events.end();
}

EventProc::WindowIterator EventProc::WindowBegin()const{
  return this->_windows.begin();
}

EventProc::WindowIterator EventProc::WindowEnd()const{
  return this->_windows.end();
}

