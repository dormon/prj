#include <waylandCPP/display.h>

waylandCPP::Display::Display(char const*name = nullptr){
  auto d = [](struct wl_display**p){wl_display_disconnect(*p);delete p;};
  display = std::shared_ptr<struct wl_display*>(new struct wl_display*,d);
  *display = wl_display_connect(name);
}

waylandCPP::Registry waylandCPP::Display::getRegistry()const{

}

egl::Display getEGLDisplay()const;
int roundtrip()const{
  return wl_display_roundtrip(*display);
}
struct wl_display*get()const{return *display;}
