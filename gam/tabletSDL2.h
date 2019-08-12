#pragma once

#include <SDL.h>
#include <SDL_syswm.h>

class TabletData{
  public:
    enum Mode{  
      NoneMode = 0,                                               
      Stylus,                                                 
      Eraser                                                  
    }active = NoneMode;
    float pressure = 0;
    float xTilt    = 0;
    float yTilt    = 0;
};

class TabletSDL2Impl;
class TabletSDL2{
  public:
    TabletSDL2(SDL_Window*window);
    ~TabletSDL2();
    bool getTabletData(TabletData*data,XEvent const&event);
  protected:
    TabletSDL2Impl*impl;
};
