#pragma once

#include<set>
#include<map>
#include<memory>
//#include<geCore/Command.h>
//#include<geCore/TypeRegister.h>

class Window{
  public:
    typedef void(*EVENTPROC)(void*eventData,void*userData/*ge::core::Accessor const&data*/);
    typedef void(*SIMPLEPROC)();
  protected:
    std::map<std::string,EVENTPROC >_eventProc ;
    std::map<std::string,SIMPLEPROC>_simpleProc;
  public:
    Window();
    virtual ~Window() = 0;
    bool hasCallback(std::string event)const;
    void setCallback(std::string event,EVENTPROC  proc = nullptr);
    void setCallback(std::string event,SIMPLEPROC proc = nullptr);
    void callCallback(std::string proc);
    void callCallback(std::string proc,void*userData);
    virtual void makeCurrent()const=0;
    virtual void swap()const=0;
};

class EventProc{
  protected:
    std::map<std::string,std::shared_ptr<Window>>_windows;
    bool _pooling = true;
    bool _running = false;
  public:
    enum KeyboardEventElem{
      KEYBOARD_TIMESTAMP = 0,
      KEYBOARD_STATE     = 1,
      KEYBOARD_REPEAT    = 2,
      KEYBOARD_SCANCODE  = 3,
      KEYBOARD_SYM       = 4,
      KEYBOARD_MODIFIERS = 5,
    };
    EventProc(bool pooling = true);
    virtual ~EventProc();
    virtual void operator()()=0;
    virtual void addWindow(std::string name,std::shared_ptr<Window>const&window)=0;
    virtual void removeWindow(std::string name)=0;
    std::shared_ptr<Window>const&getWindow(std::string name)const;
    void setCallback(std::string window,std::string event,Window::SIMPLEPROC proc = nullptr)const;
    void setCallback(std::string window,std::string event,Window::EVENTPROC proc = nullptr)const;
    void makeCurrent(std::string window)const;
    void swap(std::string window)const;
    void registerEvent(std::string name,ge::core::TypeRegister::TypeID dataType);
    typedef std::map<std::string,ge::core::TypeRegister::TypeID>::const_iterator EventIterator;
    typedef std::map<std::string,std::shared_ptr<Window>>::const_iterator WindowIterator;
    EventIterator eventBegin()const;
    EventIterator eventEnd()const;
    WindowIterator WindowBegin()const;
    WindowIterator WindowEnd()const;
};

