#pragma once

#include "util.h"
#include "wayland.hpp"

#define EGL_IMPLEMENTATION
#include "egl.hpp"

class Window{
  public:
    Window(uint32_t width,uint32_t height,bool fullscreen)
      :display     (wayland::Display())
      ,registry    (display.getRegistry())
      ,compositor  (registry.getCompositor())
      ,shell       (registry.getShell())
      ,seat        (registry.getSeat())
      ,surface     (compositor.getSurface())
      ,eglWindow   (surface.getEGLWindow(width,height))
      ,shellSurface(wayland::ShellSurface(shell,surface))
    {
      seat.addListener();
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
      configAttrib  = egl::ConfigAttrib({
          EGL_ALPHA_MASK_SIZE         ,0,
          EGL_ALPHA_SIZE              ,0           ,
          EGL_BIND_TO_TEXTURE_RGB     ,EGL_TRUE            ,
          EGL_BIND_TO_TEXTURE_RGBA    ,EGL_TRUE            ,
          EGL_BLUE_SIZE               ,4,
          EGL_BUFFER_SIZE             ,12,
          EGL_COLOR_BUFFER_TYPE       ,EGL_RGB_BUFFER      ,
          EGL_CONFIG_CAVEAT           ,EGL_NONE            ,
          EGL_CONFORMANT              ,69,
          EGL_GREEN_SIZE              ,4,
          EGL_LUMINANCE_SIZE          ,EGL_FALSE           ,
          EGL_MATCH_NATIVE_PIXMAP     ,EGL_NONE            ,
          EGL_NATIVE_RENDERABLE       ,EGL_FALSE           ,
          EGL_MAX_SWAP_INTERVAL       ,EGL_TRUE            ,
          EGL_MIN_SWAP_INTERVAL       ,EGL_FALSE           ,
          EGL_RED_SIZE                ,4,
          EGL_SAMPLE_BUFFERS          ,EGL_FALSE           ,
          EGL_SAMPLES                 ,EGL_FALSE           ,
          EGL_STENCIL_SIZE            ,0           ,
          EGL_RENDERABLE_TYPE         ,69,
          EGL_SURFACE_TYPE            ,1285,
          EGL_TRANSPARENT_TYPE        ,EGL_NONE            ,
          EGL_TRANSPARENT_RED_VALUE   ,-1,
          EGL_TRANSPARENT_GREEN_VALUE ,-1,
          EGL_TRANSPARENT_BLUE_VALUE  ,-1,
      });
      //configAttrib.setRedSize(8);
      //configAttrib.setBlueSize(8);
      //configAttrib.setGreenSize(8);
      contextAttrib = egl::ContextAttrib();
      contextAttrib.setContextMajorVersion(3);
      contextAttrib.setContextMinorVersion(1);
      context      = eglDisplay.getContext(configAttrib,contextAttrib);
      ___;
      //window       = egl::Window((NativeWindowType)surface.get(),width,height);
      ___;
      eglSurface = egl::Surface(context,eglWindow.get());
      //eglSurface   = window.getSurface(context);
      ___;
      context.makeCurrent(eglSurface);
      ___;
      bool running = true;
      ___;



      std::cerr << context.getConfig().getAttribs().toStr();

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
        //std::cerr << "ddd: " << ddd << std::endl;

        //wl_display_dispatch(ddd);
        wl_display_dispatch(ddd);
	    	//wl_display_dispatch_pending (ddd);
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
    wayland::Seat         seat;
    wayland::Surface      surface;
    wayland::ShellSurface shellSurface;
    wayland::EGLWindow    eglWindow;
    egl::Display          eglDisplay;
    egl::ConfigAttrib     configAttrib;
    egl::ContextAttrib    contextAttrib;
    egl::Context          context;
    egl::Surface          eglSurface;
};

