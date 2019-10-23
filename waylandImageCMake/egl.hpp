#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include <EGL/egl.h>
#include <wayland-egl.h>

#include "util.h"

namespace egl{
  class ConfigAttrib;
  class ContextAttrib;
  class Display;
  class Config;
  class Context;
  class Surface;
  std::string configAttribsToStr(std::vector<EGLint>const&configAttrib);
  std::vector<EGLint>getConfigAttribs(EGLDisplay const&display,EGLConfig const&config);
};

class egl::ConfigAttrib{
  public:
    ConfigAttrib(std::vector<EGLint>const&a = {});
    std::vector<EGLint>get()const;
    ConfigAttrib&set(EGLint p,EGLint v);
    ConfigAttrib&set(std::vector<EGLint>const&a);
    ConfigAttrib&setAlphaMaskSize        (EGLint v = 0                );
    ConfigAttrib&setAlphaSize            (EGLint v = 0                );
    ConfigAttrib&setBindToTextureRGB     (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setBindToTextureRGBA    (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setBlueSize             (EGLint v = 0                );
    ConfigAttrib&setBufferSize           (EGLint v = 0                );
    ConfigAttrib&setColorBufferType      (EGLint v = EGL_RGB_BUFFER   );
    ConfigAttrib&setConfigCaveat         (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setConfigID             (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setConformant           (EGLint v = 0                );
    ConfigAttrib&setDepthSize            (EGLint v = 0                );
    ConfigAttrib&setGreenSize            (EGLint v = 0                );
    ConfigAttrib&setLevel                (EGLint v = 0                );
    ConfigAttrib&setLuminanceSize        (EGLint v = 0                );
    ConfigAttrib&setMatchNativePixmap    (EGLint v = EGL_NONE         );
    ConfigAttrib&setNativeRenderable     (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setMaxSwapInterval      (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setMinSwapInterval      (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setRedSize              (EGLint v = 0                );
    ConfigAttrib&setSampleBuffers        (EGLint v = 0                );
    ConfigAttrib&setSamples              (EGLint v = 1                );
    ConfigAttrib&setStencilSize          (EGLint v = 0                );
    ConfigAttrib&setRenderableType       (EGLint v = EGL_OPENGL_ES_BIT);
    ConfigAttrib&setSurfaceType          (EGLint v = EGL_WINDOW_BIT   );
    ConfigAttrib&setTransparentType      (EGLint v = EGL_NONE         );
    ConfigAttrib&setTransparentRedValue  (EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setTransparentGreenValue(EGLint v = EGL_DONT_CARE    );
    ConfigAttrib&setTransparentBlueValue (EGLint v = EGL_DONT_CARE    );
    std::string toStr()const;
  protected:
    std::map<EGLint,EGLint>attributes;
};

class egl::ContextAttrib{
  public:
    ContextAttrib(std::vector<EGLint>const&a = {});
    std::vector<EGLint>get()const;
    ContextAttrib&set(EGLint p,EGLint v);
    ContextAttrib&set(std::vector<EGLint>const&a);
    ContextAttrib&setContextMajorVersion                   (EGLint v = 3                                  );
    ContextAttrib&setContextMinorVersion                   (EGLint v = 1                                  );
    ContextAttrib&setContextOpenGLProfileMask              (EGLint v = EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT);
    ContextAttrib&setContextOpenGLDebug                    (EGLint v = EGL_FALSE                          );
    ContextAttrib&setContextOpenGLForwardCompatible        (EGLint v = EGL_FALSE                          );
    ContextAttrib&setContextOpenGLRobustAccess             (EGLint v = EGL_FALSE                          );
    ContextAttrib&setContextOpenGLResetNotificationStrategy(EGLint v = EGL_NO_RESET_NOTIFICATION          );
  protected:
    std::map<EGLint,EGLint>attributes;
};

class egl::Display{
  public:
    Display(NativeDisplayType waylandDisplay);
    Display();
    std::vector<Config>getConfigs(ConfigAttrib const&att = {})const;
    Context getContext(Config const&c,ContextAttrib const&ctxAtt = {})const;
    Context getContext(ConfigAttrib const&confAtt = {},ContextAttrib const&ctxAtt = {})const;
    Context getContext(ContextAttrib const&ctxAtt)const;
    EGLDisplay get()const;
  protected:
    EGLint getNofCompatibleConfigs(ConfigAttrib const&att)const;
    std::vector<EGLConfig>getCompatibleConfigs(ConfigAttrib const&att,EGLint nofConfigs)const;
    std::vector<Config>getConfigs(std::vector<EGLConfig>const&configs)const;
    std::shared_ptr<EGLDisplay>display;
};



class egl::Config{
  public:
    Config(EGLConfig const&c,Display const&d);
    Config();
    EGLConfig get()const;
    ConfigAttrib getAttribs()const;
  protected:
    EGLConfig config;
    Display   display;
};

class egl::Context{
  public:
    Context(Display const&di,Config const&conf,ContextAttrib const&ctxAtt);
    Context();
    EGLContext get()const;
    Config getConfig()const;
    Display getDisplay()const;
    void makeCurrent(Surface const&surface)const;
    std::vector<EGLint>getConfigAttribs()const;
  protected:
    Display display;
    Config config;
    std::shared_ptr<EGLContext>context;
};

class egl::Surface{
  public:
    Surface(Context const&ctx,NativeWindowType window);
    Surface();
    EGLSurface get()const;
    void swap()const;
  protected:
    Context context;
    std::shared_ptr<EGLSurface>surface;
};

///////////////////////// IMPLEMENTATION //////////////////////////

#ifdef EGL_IMPLEMENTATION

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




// CONFIGATTRIB IMPL

egl::ConfigAttrib::ConfigAttrib(std::vector<EGLint>const&a){
  set(a);
}

std::vector<EGLint>egl::ConfigAttrib::get()const{
  std::vector<EGLint>result;
  for(auto const&x:attributes){
    result.push_back(x.first);
    result.push_back(x.second);
  }
  result.push_back(EGL_NONE);
  return result;
}

egl::ConfigAttrib&egl::ConfigAttrib::set(EGLint p,EGLint v){
  attributes[p] = v;
  return*this;
}

egl::ConfigAttrib&egl::ConfigAttrib::set(std::vector<EGLint>const&a){
  for(size_t i=0;i<a.size();i+=2)
    set(a[i],a[i+1]);
  return*this;
}

egl::ConfigAttrib&egl::ConfigAttrib::setAlphaMaskSize        (EGLint v){set(EGL_ALPHA_MASK_SIZE        ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setAlphaSize            (EGLint v){set(EGL_ALPHA_SIZE             ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setBindToTextureRGB     (EGLint v){set(EGL_BIND_TO_TEXTURE_RGB    ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setBindToTextureRGBA    (EGLint v){set(EGL_BIND_TO_TEXTURE_RGBA   ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setBlueSize             (EGLint v){set(EGL_BLUE_SIZE              ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setBufferSize           (EGLint v){set(EGL_BUFFER_SIZE            ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setColorBufferType      (EGLint v){set(EGL_COLOR_BUFFER_TYPE      ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setConfigCaveat         (EGLint v){set(EGL_CONFIG_CAVEAT          ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setConfigID             (EGLint v){set(EGL_CONFIG_ID              ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setConformant           (EGLint v){set(EGL_CONFORMANT             ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setDepthSize            (EGLint v){set(EGL_DEPTH_SIZE             ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setGreenSize            (EGLint v){set(EGL_GREEN_SIZE             ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setLevel                (EGLint v){set(EGL_LEVEL                  ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setLuminanceSize        (EGLint v){set(EGL_LUMINANCE_SIZE         ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setMatchNativePixmap    (EGLint v){set(EGL_MATCH_NATIVE_PIXMAP    ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setNativeRenderable     (EGLint v){set(EGL_NATIVE_RENDERABLE      ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setMaxSwapInterval      (EGLint v){set(EGL_MAX_SWAP_INTERVAL      ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setMinSwapInterval      (EGLint v){set(EGL_MIN_SWAP_INTERVAL      ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setRedSize              (EGLint v){set(EGL_RED_SIZE               ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setSampleBuffers        (EGLint v){set(EGL_SAMPLE_BUFFERS         ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setSamples              (EGLint v){set(EGL_SAMPLES                ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setStencilSize          (EGLint v){set(EGL_STENCIL_SIZE           ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setRenderableType       (EGLint v){set(EGL_RENDERABLE_TYPE        ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setSurfaceType          (EGLint v){set(EGL_SURFACE_TYPE           ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setTransparentType      (EGLint v){set(EGL_TRANSPARENT_TYPE       ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setTransparentRedValue  (EGLint v){set(EGL_TRANSPARENT_RED_VALUE  ,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setTransparentGreenValue(EGLint v){set(EGL_TRANSPARENT_GREEN_VALUE,v);return*this;}
egl::ConfigAttrib&egl::ConfigAttrib::setTransparentBlueValue (EGLint v){set(EGL_TRANSPARENT_BLUE_VALUE ,v);return*this;}

std::string egl::ConfigAttrib::toStr()const{
  return configAttribsToStr(get());
}

// CONFIGATTRIB IMPL END






// CONFIG IMPL

egl::Config::Config(EGLConfig const&c,Display const&d):config(c),display(d){}

egl::Config::Config(){}

EGLConfig egl::Config::get()const{return config;}

egl::ConfigAttrib egl::Config::getAttribs()const{
  return ConfigAttrib(egl::getConfigAttribs(display.get(),get()));
}

// CONFIG IMPL END







// CONTEXTATTRIB IMPL

egl::ContextAttrib::ContextAttrib(std::vector<EGLint>const&a){
  set(a);
}

std::vector<EGLint>egl::ContextAttrib::get()const{
  std::vector<EGLint>result;
  for(auto const&x:attributes){
    result.push_back(x.first);
    result.push_back(x.second);
  }
  result.push_back(EGL_NONE);
  return result;
}

egl::ContextAttrib&egl::ContextAttrib::set(EGLint p,EGLint v){
  attributes[p] = v;
  return*this;
}

egl::ContextAttrib&egl::ContextAttrib::set(std::vector<EGLint>const&a){
  for(size_t i=0;i<a.size();i+=2)
    set(a[i],a[i+1]);
  return*this;
}

egl::ContextAttrib&egl::ContextAttrib::setContextMajorVersion                   (EGLint v){attributes[EGL_CONTEXT_MAJOR_VERSION                     ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextMinorVersion                   (EGLint v){attributes[EGL_CONTEXT_MINOR_VERSION                     ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextOpenGLProfileMask              (EGLint v){attributes[EGL_CONTEXT_OPENGL_PROFILE_MASK               ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextOpenGLDebug                    (EGLint v){attributes[EGL_CONTEXT_OPENGL_DEBUG                      ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextOpenGLForwardCompatible        (EGLint v){attributes[EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE         ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextOpenGLRobustAccess             (EGLint v){attributes[EGL_CONTEXT_OPENGL_ROBUST_ACCESS              ] = v;return*this;}
egl::ContextAttrib&egl::ContextAttrib::setContextOpenGLResetNotificationStrategy(EGLint v){attributes[EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY] = v;return*this;}

// CONTEXTATTRIB IMPL END



// DISPLAY IMPL

egl::Display::Display(NativeDisplayType waylandDisplay){
  auto d = [](EGLDisplay*p){eglTerminate(*p);delete p;};
  display = std::shared_ptr<EGLDisplay>(new EGLDisplay,d);
  EGL_CALLV(*display,eglGetDisplay,waylandDisplay);
  EGL_CALL(eglInitialize,*display,nullptr,nullptr);
}

egl::Display::Display(){}

std::vector<egl::Config>egl::Display::getConfigs(ConfigAttrib const&att)const{
  auto const nofConfigs = getNofCompatibleConfigs(att);
  std::cerr << "nofConfigs: " << nofConfigs << std::endl;
  auto const configs = getCompatibleConfigs(att,nofConfigs);
  //for(auto const&c:configs){
  //  std::cerr << "################################" << std::endl;
  //  egl::printConfigAttribs(getConfigAttribs(get(),c));
  //}
  return getConfigs(configs);
}

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

EGLDisplay egl::Display::get()const{
  return *display;
}

std::string configAttribToString(EGLint v){
  switch(v){
    case EGL_ALPHA_MASK_SIZE        :return"EGL_ALPHA_MASK_SIZE        ";
    case EGL_ALPHA_SIZE             :return"EGL_ALPHA_SIZE             ";
    case EGL_BIND_TO_TEXTURE_RGB    :return"EGL_BIND_TO_TEXTURE_RGB    ";
    case EGL_BIND_TO_TEXTURE_RGBA   :return"EGL_BIND_TO_TEXTURE_RGBA   ";
    case EGL_BLUE_SIZE              :return"EGL_BLUE_SIZE              ";
    case EGL_BUFFER_SIZE            :return"EGL_BUFFER_SIZE            ";
    case EGL_COLOR_BUFFER_TYPE      :return"EGL_COLOR_BUFFER_TYPE      ";
    case EGL_CONFIG_CAVEAT          :return"EGL_CONFIG_CAVEAT          ";
    case EGL_CONFIG_ID              :return"EGL_CONFIG_ID              ";
    case EGL_CONFORMANT             :return"EGL_CONFORMANT             ";
    case EGL_DEPTH_SIZE             :return"EGL_DEPTH_SIZE             ";
    case EGL_GREEN_SIZE             :return"EGL_GREEN_SIZE             ";
    case EGL_LEVEL                  :return"EGL_LEVEL                  ";
    case EGL_LUMINANCE_SIZE         :return"EGL_LUMINANCE_SIZE         ";
    case EGL_MATCH_NATIVE_PIXMAP    :return"EGL_MATCH_NATIVE_PIXMAP    ";
    case EGL_NATIVE_RENDERABLE      :return"EGL_NATIVE_RENDERABLE      ";
    case EGL_MAX_SWAP_INTERVAL      :return"EGL_MAX_SWAP_INTERVAL      ";
    case EGL_MIN_SWAP_INTERVAL      :return"EGL_MIN_SWAP_INTERVAL      ";
    case EGL_RED_SIZE               :return"EGL_RED_SIZE               ";
    case EGL_SAMPLE_BUFFERS         :return"EGL_SAMPLE_BUFFERS         ";
    case EGL_SAMPLES                :return"EGL_SAMPLES                ";
    case EGL_STENCIL_SIZE           :return"EGL_STENCIL_SIZE           ";
    case EGL_RENDERABLE_TYPE        :return"EGL_RENDERABLE_TYPE        ";
    case EGL_SURFACE_TYPE           :return"EGL_SURFACE_TYPE           ";
    case EGL_TRANSPARENT_TYPE       :return"EGL_TRANSPARENT_TYPE       ";
    case EGL_TRANSPARENT_RED_VALUE  :return"EGL_TRANSPARENT_RED_VALUE  ";
    case EGL_TRANSPARENT_GREEN_VALUE:return"EGL_TRANSPARENT_GREEN_VALUE";
    case EGL_TRANSPARENT_BLUE_VALUE :return"EGL_TRANSPARENT_BLUE_VALUE ";
    default:break;
  }
  std::stringstream ss;
  ss << v;
  return ss.str();
}

std::string configAttribValueToString(EGLint v){
  switch(v){
    case EGL_TRUE            :return"EGL_TRUE            ";
    case EGL_FALSE           :return"EGL_FALSE           ";
    case EGL_RGB_BUFFER      :return"EGL_RGB_BUFFER      ";
    case EGL_LUMINANCE_BUFFER:return"EGL_LUMINANCE_BUFFER";
    case EGL_NONE            :return"EGL_NONE            ";
    case EGL_SLOW_CONFIG     :return"EGL_SLOW_CONFIG     ";
    //case EGL_NON_CONFORMANT  :return"EGL_NON_CONFORMANT  ";
    case EGL_TRANSPARENT_RGB :return"EGL_TRANSPARENT_RGB ";
    default:break;
  }

  std::stringstream ss;
  ss << v;
  return ss.str();
}


EGLint attt[] = {
  EGL_ALPHA_MASK_SIZE         ,0,
  EGL_ALPHA_SIZE              ,0           ,
  EGL_BIND_TO_TEXTURE_RGB     ,EGL_TRUE            ,
  EGL_BIND_TO_TEXTURE_RGBA    ,EGL_TRUE            ,
  EGL_BLUE_SIZE               ,4,
  EGL_BUFFER_SIZE             ,12,
  EGL_COLOR_BUFFER_TYPE       ,EGL_RGB_BUFFER      ,
  EGL_CONFIG_CAVEAT           ,EGL_NONE            ,
  EGL_CONFORMANT              ,69,
   EGL_DEPTH_SIZE             ,0,
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
  EGL_NONE,
};

EGLint egl::Display::getNofCompatibleConfigs(ConfigAttrib const&att)const{
  EGLint nofConfigs = 0;

  std::cerr << egl::configAttribsToStr(att.get());

  EGL_CALL(eglChooseConfig,*display,/*att.get().data()*/attt,nullptr,0,&nofConfigs);
  return nofConfigs;
}

std::vector<EGLConfig>egl::Display::getCompatibleConfigs(ConfigAttrib const&att,EGLint nofConfigs)const{
  std::vector<EGLConfig>configs(nofConfigs);
  configs.resize(nofConfigs);
  EGL_CALL(eglChooseConfig,*display,/*att.get().data()*/attt,configs.data(),configs.size(),&nofConfigs);
  return configs;
}

std::vector<egl::Config>egl::Display::getConfigs(std::vector<EGLConfig>const&configs)const{
  std::vector<Config>result;
  for(auto const&c:configs)
    result.emplace_back(c,*this);
  return result;
}

// DISPLAY IMPL END




// CONTEXT IMPL

egl::Context::Context(Display const&di,Config const&conf,ContextAttrib const&ctxAtt):display(di),config(conf){
  auto d = [](EGLContext*p){delete p;};
  context = std::shared_ptr<EGLContext>(new EGLContext,d);
  EGL_CALL(eglBindAPI,EGL_OPENGL_ES_API);
  EGL_CALLV(*context,eglCreateContext,di.get(),config.get(),EGL_NO_CONTEXT,ctxAtt.get().data());
}

egl::Context::Context(){}

EGLContext egl::Context::get()const{return *context;}

egl::Config egl::Context::getConfig()const{return config;}

egl::Display egl::Context::getDisplay()const{return display;}

void egl::Context::makeCurrent(Surface const&surface)const{
  EGL_CALL(eglMakeCurrent,display.get(),surface.get(),surface.get(),*context);
}

std::vector<EGLint>egl::getConfigAttribs(EGLDisplay const&display,EGLConfig const&config){
  std::vector<EGLint>const att = {
    EGL_ALPHA_MASK_SIZE        ,
    EGL_ALPHA_SIZE             ,
    EGL_BIND_TO_TEXTURE_RGB    ,
    EGL_BIND_TO_TEXTURE_RGBA   ,
    EGL_BLUE_SIZE              ,
    EGL_BUFFER_SIZE            ,
    EGL_COLOR_BUFFER_TYPE      ,
    EGL_CONFIG_CAVEAT          ,
    EGL_CONFIG_ID              ,
    EGL_CONFORMANT             ,
    EGL_DEPTH_SIZE             ,
    EGL_GREEN_SIZE             ,
    EGL_LEVEL                  ,
    EGL_LUMINANCE_SIZE         ,
    EGL_MATCH_NATIVE_PIXMAP    ,
    EGL_NATIVE_RENDERABLE      ,
    EGL_MAX_SWAP_INTERVAL      ,
    EGL_MIN_SWAP_INTERVAL      ,
    EGL_RED_SIZE               ,
    EGL_SAMPLE_BUFFERS         ,
    EGL_SAMPLES                ,
    EGL_STENCIL_SIZE           ,
    EGL_RENDERABLE_TYPE        ,
    EGL_SURFACE_TYPE           ,
    EGL_TRANSPARENT_TYPE       ,
    EGL_TRANSPARENT_RED_VALUE  ,
    EGL_TRANSPARENT_GREEN_VALUE,
    EGL_TRANSPARENT_BLUE_VALUE ,
  };

  std::vector<EGLint>result;
  for(auto const&a:att){
    EGLint v;
    EGL_CALL(eglGetConfigAttrib,display,config,a,&v);
    result.push_back(a);
    result.push_back(v);
  }
  return result;
}

std::vector<EGLint>egl::Context::getConfigAttribs()const{
  return egl::getConfigAttribs(getDisplay().get(),getConfig().get());
}

// CONTEXT IMPL END





// SURFACE IMPL

egl::Surface::Surface(Context const&ctx,NativeWindowType window):context(ctx){
  auto d = [&](EGLSurface*p){eglDestroySurface(ctx.getDisplay().get(),p);delete p;};
  surface = std::shared_ptr<EGLSurface>(new EGLSurface,d);
  EGL_CALLV(*surface,eglCreateWindowSurface,context.getDisplay().get(),context.getConfig().get(),window,NULL);
}

egl::Surface::Surface(){}

EGLSurface egl::Surface::get()const{return*surface;}

void egl::Surface::swap()const{
  EGL_CALL(eglSwapBuffers,context.getDisplay().get(), *surface);
}

// SURFACE IMPL END

std::string egl::configAttribsToStr(std::vector<EGLint>const&configAttrib){
  std::stringstream ss;
  for(size_t i=0;i<configAttrib.size();i+=2){
    ss << configAttribToString(configAttrib.at(i));
    if(i+1<configAttrib.size())ss << " " << configAttribValueToString(configAttrib.at(i+1));
    ss << std::endl;
  }
  return ss.str();
}

#endif
