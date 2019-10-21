#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <EGL/egl.h>
#include <wayland-egl.h>

#include "util.h"

namespace egl{
  class Display;
  class ConfigAttrib;
  class Config;
  class ContextAttrib;
  class Context;
  class Window;
  class Surface;
};


#define EGL_CALL(fce,...)\
  fce(__VA_ARGS__);\
  if(auto err = eglGetError(); err != EGL_SUCCESS)\
    throw std::runtime_error(std::string(__FILE__)+"/"+STRINGIZE(__LINE__)+" "+std::string(#fce)+" - "+eglErrorToStr(err))

#define EGL_CALLV(v,fce,...)\
  v=fce(__VA_ARGS__);\
  if(auto err = eglGetError(); err != EGL_SUCCESS)\
    throw std::runtime_error(std::string(__FILE__)+"/"+STRINGIZE(__LINE__)+" "+std::string(#fce)+" - "+eglErrorToStr(err))


std::string eglErrorToStr(EGLint err){
  switch(err){
    case EGL_SUCCESS            :return "EGL_SUCCESS             - The last function succeeded without error.";
    case EGL_NOT_INITIALIZED    :return "EGL_NOT_INITIALIZED     - EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
    case EGL_BAD_ACCESS         :return "EGL_BAD_ACCESS          - EGL cannot access a requested resource (for example a context is bound in another thread).";
    case EGL_BAD_ALLOC          :return "EGL_BAD_ALLOC           - EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE      :return "EGL_BAD_ATTRIBUTE       - An unrecognized attribute or attribute value was passed in the attribute list.";
    case EGL_BAD_CONTEXT        :return "EGL_BAD_CONTEXT         - An EGLContext argument does not name a valid EGL rendering context.";
    case EGL_BAD_CONFIG         :return "EGL_BAD_CONFIG          - An EGLConfig argument does not name a valid EGL frame buffer configuration.";
    case EGL_BAD_CURRENT_SURFACE:return "EGL_BAD_CURRENT_SURFACE - The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY        :return "EGL_BAD_DISPLAY         - An EGLDisplay argument does not name a valid EGL display connection.";
    case EGL_BAD_SURFACE        :return "EGL_BAD_SURFACE         - An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH          :return "EGL_BAD_MATCH           - Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER      :return "EGL_BAD_PARAMETER       - One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP  :return "EGL_BAD_NATIVE_PIXMAP   - A NativePixmapType argument does not refer to a valid native pixmap.";
    case EGL_BAD_NATIVE_WINDOW  :return "EGL_BAD_NATIVE_WINDOW   - A NativeWindowType argument does not refer to a valid native window.";
    case EGL_CONTEXT_LOST       :return "EGL_CONTEXT_LOST        - A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
  }
  return "";
}

class egl::ConfigAttrib{
  public:
    ConfigAttrib(std::vector<EGLint>const&a = {}){
      set(a);
    }
    std::vector<EGLint>get()const{
      std::vector<EGLint>result;
      for(auto const&x:attributes){
        result.push_back(x.first);
        result.push_back(x.second);
      }
      result.push_back(EGL_NONE);
      return result;
    }
    ConfigAttrib&set(EGLint p,EGLint v){
      attributes[p] = v;
      return*this;
    }
    ConfigAttrib&set(std::vector<EGLint>const&a){
      for(size_t i=0;i<a.size();i+=2)
        set(a[i],a[i+1]);
      return*this;
    }
    ConfigAttrib&setAlphaMaskSize        (EGLint v = 0                ){set(EGL_ALPHA_MASK_SIZE        ,v);return*this;}
    ConfigAttrib&setAlphaSize            (EGLint v = 0                ){set(EGL_ALPHA_SIZE             ,v);return*this;}
    ConfigAttrib&setBindToTextureRGB     (EGLint v = EGL_DONT_CARE    ){set(EGL_BIND_TO_TEXTURE_RGB    ,v);return*this;}
    ConfigAttrib&setBindToTextureRGBA    (EGLint v = EGL_DONT_CARE    ){set(EGL_BIND_TO_TEXTURE_RGBA   ,v);return*this;}
    ConfigAttrib&setBlueSize             (EGLint v = 0                ){set(EGL_BLUE_SIZE              ,v);return*this;}
    ConfigAttrib&setBufferSize           (EGLint v = 0                ){set(EGL_BUFFER_SIZE            ,v);return*this;}
    ConfigAttrib&setColorBufferType      (EGLint v = EGL_RGB_BUFFER   ){set(EGL_BUFFER_SIZE            ,v);return*this;}
    ConfigAttrib&setConfigCaveat         (EGLint v = EGL_DONT_CARE    ){set(EGL_CONFIG_CAVEAT          ,v);return*this;}
    ConfigAttrib&setConformant           (EGLint v = 0                ){set(EGL_CONFORMANT             ,v);return*this;}
    ConfigAttrib&setGreenSize            (EGLint v = 0                ){set(EGL_GREEN_SIZE             ,v);return*this;}
    ConfigAttrib&setLuminanceSize        (EGLint v = 0                ){set(EGL_LUMINANCE_SIZE         ,v);return*this;}
    ConfigAttrib&setMatchNativePixmap    (EGLint v = EGL_NONE         ){set(EGL_MATCH_NATIVE_PIXMAP    ,v);return*this;}
    ConfigAttrib&setNativeRenderable     (EGLint v = EGL_DONT_CARE    ){set(EGL_NATIVE_RENDERABLE      ,v);return*this;}
    ConfigAttrib&setMaxSwapInterval      (EGLint v = EGL_DONT_CARE    ){set(EGL_MAX_SWAP_INTERVAL      ,v);return*this;}
    ConfigAttrib&setMinSwapInterval      (EGLint v = EGL_DONT_CARE    ){set(EGL_MIN_SWAP_INTERVAL      ,v);return*this;}
    ConfigAttrib&setRedSize              (EGLint v = 0                ){set(EGL_RED_SIZE               ,v);return*this;}
    ConfigAttrib&setSampleBuffers        (EGLint v = 0                ){set(EGL_SAMPLE_BUFFERS         ,v);return*this;}
    ConfigAttrib&setSamples              (EGLint v = 1                ){set(EGL_SAMPLES                ,v);return*this;}
    ConfigAttrib&setStencilSize          (EGLint v = 0                ){set(EGL_STENCIL_SIZE           ,v);return*this;}
    ConfigAttrib&setRenderableType       (EGLint v = EGL_OPENGL_ES_BIT){set(EGL_RENDERABLE_TYPE        ,v);return*this;}
    ConfigAttrib&setSurfaceType          (EGLint v = EGL_WINDOW_BIT   ){set(EGL_SURFACE_TYPE           ,v);return*this;}
    ConfigAttrib&setTransparentType      (EGLint v = EGL_NONE         ){set(EGL_TRANSPARENT_TYPE       ,v);return*this;}
    ConfigAttrib&setTransparentRedValue  (EGLint v = EGL_DONT_CARE    ){set(EGL_TRANSPARENT_RED_VALUE  ,v);return*this;}
    ConfigAttrib&setTransparentGreenValue(EGLint v = EGL_DONT_CARE    ){set(EGL_TRANSPARENT_GREEN_VALUE,v);return*this;}
    ConfigAttrib&setTransparentBlueValue (EGLint v = EGL_DONT_CARE    ){set(EGL_TRANSPARENT_BLUE_VALUE ,v);return*this;}
  protected:
    std::map<EGLint,EGLint>attributes;
};

class egl::Config{
  public:
    Config(EGLConfig const&c):config(c){}
    Config(){}
    //Config(Display const&di,ConfigAttrib const&att = {}){
    //  auto attributes = att.get();
    //}
    EGLConfig get()const{return config;}
  protected:
    EGLConfig config;
};

class egl::ContextAttrib{
  public:
    ContextAttrib(std::vector<EGLint>const&a = {}){
      set(a);
    }
    std::vector<EGLint>get()const{
      std::vector<EGLint>result;
      for(auto const&x:attributes){
        result.push_back(x.first);
        result.push_back(x.second);
      }
      result.push_back(EGL_NONE);
      return result;
    }
    ContextAttrib&set(EGLint p,EGLint v){
      attributes[p] = v;
      return*this;
    }
    ContextAttrib&set(std::vector<EGLint>const&a){
      for(size_t i=0;i<a.size();i+=2)
        set(a[i],a[i+1]);
      return*this;
    }
    ContextAttrib&setContextMajorVersion                   (EGLint v = 3                                  ){attributes[EGL_CONTEXT_MAJOR_VERSION                     ] = v;return*this;}
    ContextAttrib&setContextMinorVersion                   (EGLint v = 1                                  ){attributes[EGL_CONTEXT_MINOR_VERSION                     ] = v;return*this;}
    ContextAttrib&setContextOpenGLProfileMask              (EGLint v = EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT){attributes[EGL_CONTEXT_OPENGL_PROFILE_MASK               ] = v;return*this;}
    ContextAttrib&setContextOpenGLDebug                    (EGLint v = EGL_FALSE                          ){attributes[EGL_CONTEXT_OPENGL_DEBUG                      ] = v;return*this;}
    ContextAttrib&setContextOpenGLForwardCompatible        (EGLint v = EGL_FALSE                          ){attributes[EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE         ] = v;return*this;}
    ContextAttrib&setContextOpenGLRobustAccess             (EGLint v = EGL_FALSE                          ){attributes[EGL_CONTEXT_OPENGL_ROBUST_ACCESS              ] = v;return*this;}
    ContextAttrib&setContextOpenGLResetNotificationStrategy(EGLint v = EGL_NO_RESET_NOTIFICATION          ){attributes[EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY] = v;return*this;}
  protected:
    std::map<EGLint,EGLint>attributes;
};

class egl::Display{
  public:
    Display(NativeDisplayType waylandDisplay){
      auto d = [](EGLDisplay*p){eglTerminate(*p);delete p;};
      display = std::shared_ptr<EGLDisplay>(new EGLDisplay,d);
      EGL_CALLV(*display,eglGetDisplay,waylandDisplay);
      EGL_CALL(eglInitialize,*display,nullptr,nullptr);
    }
    Display(){}
    std::vector<Config>getConfigs(ConfigAttrib const&att = {})const{
      auto const nofConfigs = getNofCompatibleConfigs(att);
      auto const configs = getCompatibleConfigs(att,nofConfigs);
      return getConfigs(configs);
    }
    Context getContext(Config const&c,ContextAttrib const&ctxAtt = {})const;
    Context getContext(ConfigAttrib const&confAtt = {},ContextAttrib const&ctxAtt = {})const;
    Context getContext(ContextAttrib const&ctxAtt)const;
    EGLDisplay get()const{
      return *display;
    }
  protected:
    EGLint getNofCompatibleConfigs(ConfigAttrib const&att)const{
      EGLint nofConfigs = 0;
      if(eglChooseConfig(*display,att.get().data(),nullptr,0,&nofConfigs) != EGL_TRUE)
        throw std::runtime_error("eglChooseConfig failed - getting number of compatible configs");
      return nofConfigs;
    }
    std::vector<EGLConfig>getCompatibleConfigs(ConfigAttrib const&att,EGLint nofConfigs)const{
      std::vector<EGLConfig>configs(nofConfigs);
      configs.resize(nofConfigs);
      if(eglChooseConfig(*display,att.get().data(),configs.data(),configs.size(),&nofConfigs) != EGL_TRUE)
        throw std::runtime_error("eglChooseConfig failed - getting compatible configs");
      return configs;
    }
    std::vector<Config>getConfigs(std::vector<EGLConfig>const&configs)const{
      std::vector<Config>result;
      for(auto const&c:configs)
        result.emplace_back(c);
      return result;
    }
    std::shared_ptr<EGLDisplay>display;
};

class egl::Context{
  public:
    Context(Display const&di,Config const&conf,ContextAttrib const&ctxAtt):display(di),config(conf){
      auto d = [](EGLContext*p){delete p;};
      context = std::shared_ptr<EGLContext>(new EGLContext,d);
      EGL_CALL(eglBindAPI,EGL_OPENGL_ES_API);
      EGL_CALLV(*context,eglCreateContext,di.get(),config.get(),EGL_NO_CONTEXT,ctxAtt.get().data());
    }
    Context(){}
    EGLContext get()const{return *context;}
    Config getConfig()const{return config;}
    Display getDisplay()const{return display;}
    void makeCurrent(Surface const&surface)const;
  protected:
    Display display;
    Config config;
    std::shared_ptr<EGLContext>context;
};

egl::Context egl::Display::getContext(egl::Config const&conf,egl::ContextAttrib const&ctxAtt)const{
  return Context(*this,conf,ctxAtt);
}

egl::Context egl::Display::getContext(ConfigAttrib const&confAtt,ContextAttrib const&ctxAtt)const{
  auto const confs = getConfigs(confAtt);
  if(confs.empty())std::runtime_error("egl::Display::getContext() - getConfigs returned zero number of configs");
  return getContext(confs[0],ctxAtt);
}

egl::Context egl::Display::getContext(ContextAttrib const&ctxAtt)const{
  auto const confs = getConfigs();
  if(confs.empty())std::runtime_error("egl::Display::getContext() - getConfigs returned zero number of configs");
  return getContext(confs[0],ctxAtt);
}

class egl::Window{
  public:
    Window(NativeWindowType waylandSurface,uint32_t width,uint32_t height){
      auto d = [](struct wl_egl_window**p){wl_egl_window_destroy(*p);delete p;};
      window = std::shared_ptr<NativeWindowType>(new NativeWindowType,d);
	    *window = (NativeWindowType)wl_egl_window_create ((struct wl_surface*)waylandSurface, width, height);
    }
    Window(){}
    Surface getSurface(Context const&ctx)const;
    NativeWindowType get()const{return*window;}
  protected:
    std::shared_ptr<NativeWindowType>window;
};

class egl::Surface{
  public:
    Surface(Context const&ctx,Window const&window):context(ctx){
      auto d = [&](EGLSurface*p){eglDestroySurface(ctx.getDisplay().get(),p);delete p;};
      surface = std::shared_ptr<EGLSurface>(new EGLSurface,d);
	    EGL_CALLV(*surface,eglCreateWindowSurface,context.getDisplay().get(),context.getConfig().get(),window.get(),NULL);
    }
    Surface(){}
    EGLSurface get()const{return*surface;}
    void swap()const{
	    EGL_CALL(eglSwapBuffers,context.getDisplay().get(), *surface);
    }
  protected:
    Context context;
    std::shared_ptr<EGLSurface>surface;
};

void egl::Context::makeCurrent(Surface const&surface)const{
  EGL_CALL(eglMakeCurrent,display.get(),surface.get(),surface.get(),*context);
}

egl::Surface egl::Window::getSurface(Context const&ctx)const{
  return Surface(ctx,*this);
}

