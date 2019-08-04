#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <Vars/Vars.h>

#include <SDL.h>
#include <SDL_syswm.h>
#include <GL/glew.h>
#include <createWindow.h>
#include <destroyWindow.h>
#include <X11/extensions/XInput.h>


class Dev{
  public:
    Dev(XDevice const&d,XID i):device(d),id(i){}
    XDevice device;
    XID id;
};

std::vector<Dev>getDevices(Display*dpy){
  std::vector<Dev>result;
  int ndev;
  auto devInfo = XListInputDevices(dpy,&ndev);
  std::cerr << ndev << std::endl;
  for(int i=0;i<ndev;++i){
    auto openDev = XOpenDevice(dpy,devInfo[i].id);
    result.emplace_back(openDev,devInfo[i].id);

  }
  int sid = 15;
  
  auto stylus = XOpenDevice(dpy,sid);
  return result;
}

int main(){
  vars::Vars vars;

  createWindow(vars);

  auto&running = vars.addBool("mainLoopRunning",true);


  auto window = *vars.get<SDL_Window*>("window");
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  SDL_GetWindowWMInfo(window,&info);
  auto dpy = info.info.x11.display;
  auto ww  = info.info.x11.window;

  //XSelectInput(dpy,ww,0);
  XEvent ee;
  int ndev;
  auto devInfo = XListInputDevices(dpy,&ndev);
  std::cerr << ndev << std::endl;
  for(int i=0;i<ndev;++i)
    std::cerr << i << " " << devInfo[i].name << " id: " << devInfo[i].id << std::endl;
  int sid = 15;
  auto stylus = XOpenDevice(dpy,sid);

  XAnyClassPtr ici = devInfo[sid].inputclassinfo;
  for(int j=0;j<stylus->num_classes;++j){
    if(ici->c_class == ValuatorClass){
      XValuatorInfo *xvi = (XValuatorInfo *)ici;
      std::cerr << "pressure levels: " << xvi->axes[2].max_value << std::endl;
      if (xvi->num_axes > 3) {                                     
        /* this is assuming that the tablet has the same tilt resolution in both
         * positive and negative directions. It would be rather weird if it didn't.. */
        std::cerr << "XtiltLevels: " << xvi->axes[3].max_value << std::endl;
        std::cerr << "YtiltLevels: " << xvi->axes[4].max_value << std::endl;
      }                                                            


    }
    ici = (XAnyClassPtr)(((char *)ici) + ici->length); 
  }


  SDL_Event event;
  while(running){
    XPeekEvent(dpy,&ee);
    //XCheckWindowEvent(dpy, ww, 0, &ee);
    if(ee.type == MotionNotify){// ButtonPress){
      auto eee = (XDeviceMotionEvent*)&ee;


      const unsigned char axis_first = eee->first_axis;
      const unsigned char axes_end = axis_first + eee->axes_count; /* after the last */
      int axis_value;

      /* Note: This event might be generated with incomplete dataset
       * (don't exactly know why, looks like in some cases, if the value does not change,
       * it is not included in subsequent XDeviceMotionEvent events).
       * So we have to check which values this event actually contains!
       */

#  define AXIS_VALUE_GET(axis, val) \
    ((axis_first <= axis && axes_end > axis) && \
     ((void)(val = eee->axis_data[axis - axis_first]), true))

     if (AXIS_VALUE_GET(2, axis_value)) {
      std::cerr << "motion: " << eee->deviceid << " " << axis_value << std::endl;
     }
    }
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          std::cerr << "keydown" << std::endl;
          break;
        case SDL_KEYUP:
          break;
        case SDL_MOUSEMOTION:
          break;
        case SDL_MOUSEBUTTONDOWN:
          break;
        case SDL_MOUSEBUTTONUP:
          break;
        case SDL_FINGERUP:
          std::cerr << "finger up" << std::endl;
          break;
        case SDL_FINGERDOWN:
          std::cerr << "finger down" << std::endl;
          break;
        case SDL_FINGERMOTION:
          std::cerr << "finger motion" << std::endl;
          break;
      }
    }

    glClearColor(0,0.05,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(*vars.get<SDL_Window*>("window"));
  //std::cerr << "GetNumTouchFingers:" << SDL_GetNumTouchFingers(0) << std::endl;
  //std::cerr << "GetNumTouch: " << SDL_GetNumTouchDevices() << std::endl;

  }

  destroyWindow(vars);

  return 0;
}


