#pragma once

#include "util.h"

class Window{
  public:
    Window(uint32_t width,uint32_t height,bool fullscreen)
      :display     (wayland::Display())
      ,registry    (display.getRegistry())
      ,compositor  (registry.getCompositor())
      ,shell       (registry.getShell())
      ,surface     (compositor.getSurface())
      ,shellSurface(wayland::ShellSurface(shell,surface))
    {
      //std::cerr << "Window::Window()" << std::endl;
      //___;
      //display      = wayland::Display();
      //___;
      //registry     = display.getRegistry();
      //___;
      //compositor   = registry.getCompositor();
      //___;
      //shell        = registry.getShell();
      //___;
      //surface      = compositor.getSurface();
      //___;
      //shellSurface = wayland::ShellSurface(shell,surface);
      if(fullscreen)
        shellSurface.toggleFullscreen();
      ___;
      eglDisplay   = egl::Display(display.get());
      ___;
      configAttrib  = egl::ConfigAttrib();
      configAttrib.setRedSize(8);
      configAttrib.setBlueSize(8);
      configAttrib.setGreenSize(8);
      contextAttrib = egl::ContextAttrib();
      contextAttrib.setContextMajorVersion(3);
      contextAttrib.setContextMinorVersion(1);
      context      = eglDisplay.getContext(configAttrib,contextAttrib);
      ___;
      window       = egl::Window(surface.get(),width,height);
      ___;
      eglSurface   = window.getSurface(context);
      ___;
      context.makeCurrent(eglSurface);
      ___;
      bool running = true;
      ___;
    }
    virtual ~Window(){
      std::cerr << "Window::Window" << std::endl;
    }
    
    virtual void onInit(){}
    virtual void onExit(){}
    virtual void onDraw(){}

    void start(){
      ___;
      onInit();
      ___;
      while(running){
        ___;
        auto ddd = display.get();
        ___;
        std::cerr << "ddd: " << ddd << std::endl;

        //wl_display_dispatch(ddd);
	    	wl_display_dispatch_pending (ddd);
        ___;
        onDraw();
        ___;
      }
      ___;
      onExit();
      ___;
    }
  protected:
    bool running = true;
    wayland::Display      display;
    wayland::Registry     registry;
    wayland::Compositor   compositor;
    wayland::Shell        shell;
    wayland::Surface      surface;
    wayland::ShellSurface shellSurface;
    egl::Display          eglDisplay;
    egl::ConfigAttrib     configAttrib;
    egl::ContextAttrib    contextAttrib;
    egl::Context          context;
    egl::Window           window;
    egl::Surface          eglSurface;
};

