#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <vulkan/vulkan.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl


int main(){
  Display*dpy = XOpenDisplay(nullptr);
  int ndev;
  auto devInfo = XListInputDevices(dpy,&ndev);
  std::cerr << ndev << std::endl;
  for(int i=0;i<ndev;++i)
    std::cerr << i << " " << devInfo[i].name << std::endl;

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

  XSelectInput(dpy,0,NoEventMask);
  XEvent event;
  
  while (true)
  {
    XPointer a;
    XCheckIfEvent(dpy,&event,[](Display*,XEvent*,XPointer){return 1;},a);
    std::cerr << event.type << std::endl;
      //switch (event.type)
      //{
      //    case Expose:
      //        printf("Expose\n");
      //        break;
  
      //    default:
      //        break;
      //}
  }



//  XEvent xevent;                                                       
//
//  size_t counter = 0;
//  while(counter < 10){
//  while(XPending(dpy)){
//    XNextEvent(dpy, &xevent);
//    counter ++;
//
//  }
//  }
  //XNextEvent(dpy, &xevent); 

//#define AXIS_VALUE_GET(axis, val)\
//  ((axis_first <= axis && axes_end > axis) && \
//  ((void)(val = data->axis_data[axis - axis_first]), true))             
//                                                                           
//          if (AXIS_VALUE_GET(2, axis_value)) {                             
//            window->GetTabletData()->Pressure = axis_value / ((float)xtablet.PressureLevels);
//          } 
//


  XCloseDevice(dpy,stylus);

  XFreeDeviceList(devInfo);


  XCloseDisplay(dpy);

  return 0;
}


