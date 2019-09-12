#pragma once

#include <waylandCPP/fwd.h>

class wayland::Display{
  public:
    Display(char const*name = nullptr){
      auto d = [](struct wl_display**p){wl_display_disconnect(*p);delete p;};
      display = std::shared_ptr<struct wl_display*>(new struct wl_display*,d);
      *display = wl_display_connect(name);
    }
    Registry getRegistry()const;
    egl::Display getEGLDisplay()const;
    int roundtrip()const{
      return wl_display_roundtrip(*display);
    }
    struct wl_display*get()const{return *display;}
  protected:
    std::shared_ptr<struct wl_display*>display;
};

