#pragma once

#include <memory>
#include <iostream>

#include <string.h>

#include <wayland-client.h>
#include <wayland-cursor.h>

#include "util.h"

namespace wayland{
  class Display;
  class Registry;
  class Compositor;
  class Shell;
  class Seat;
  class Surface;
  class ShellSurface;
};

#define WAYLAND_CALLV(v,fce,...)\
  if((v=fce(__VA_ARGS__)) == nullptr)\
    throw std::runtime_error(std::string(__FILE__)+"/"+STRINGIZE(__LINE__)+" "+std::string(#fce)+" - "+strerror(errno))

#define WAYLAND_CALLR(fce,...)\
  if(fce(__VA_ARGS__) < 0)\
    throw std::runtime_error(std::string(__FILE__)+"/"+STRINGIZE(__LINE__)+" "+std::string(#fce)+" - "+strerror(errno))

class wayland::Display{
  public:
    Display(char const*name = nullptr){
      auto d = [](struct wl_display**p){wl_display_disconnect(*p);delete p;};
      display = std::shared_ptr<struct wl_display*>(new struct wl_display*,d);
      WAYLAND_CALLV(*display,wl_display_connect,name);
    }
    Registry getRegistry()const;
    int roundtrip()const{
      return wl_display_roundtrip(*display);
    }
    struct wl_display*get()const{return *display;}
  protected:
    std::shared_ptr<struct wl_display*>display;
};

class wayland::Compositor{
  public:
    Compositor(std::shared_ptr<struct wl_compositor*>c):compositor(c){}
    Compositor(){}
    auto get()const{
      return *compositor;
    }
    bool isReady()const{
      return compositor != nullptr && *compositor != nullptr;
    }
    Surface getSurface()const;
  protected:
    std::shared_ptr<struct wl_compositor*>compositor;
};

class wayland::Shell{
  public:
    Shell(std::shared_ptr<struct wl_shell*>const&s):shell(s){}
    Shell(){}
    auto get()const{
      return *shell;
    }
  protected:
    std::shared_ptr<struct wl_shell*>shell;
};

class wayland::Seat{
  public:
    Seat(std::shared_ptr<struct wl_seat*>const&s):seat(s){}
    Seat(){}
    auto get()const{
      return *seat;
    }
    void addListener(){
      WAYLAND_CALLR(wl_seat_add_listener,*seat, &seat_listener,*seat);
    }
  protected:
    std::shared_ptr<struct wl_seat*>seat;

    //struct wl_pointer *pointer;
	  struct wl_keyboard *keyboard;
    //struct wl_cursor_theme *cursor_theme;
	  //struct wl_cursor *default_cursor;
	  //struct wl_surface *cursor_surface;

    //static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
    //		     uint32_t serial, struct wl_surface *surface,
    //		     wl_fixed_t sx, wl_fixed_t sy)
    //{
    //	Seat*display = (Seat*)data;
    //	struct wl_buffer *buffer;
    //	struct wl_cursor *cursor = display->default_cursor;
    //	struct wl_cursor_image *image;
    //
    //	if (display->window->fullscreen)
    //		wl_pointer_set_cursor(pointer, serial, NULL, 0, 0);
    //	else if (cursor) {
    //		image = display->default_cursor->images[0];
    //		buffer = wl_cursor_image_get_buffer(image);
    //		wl_pointer_set_cursor(pointer, serial,
    //				      display->cursor_surface,
    //				      image->hotspot_x,
    //				      image->hotspot_y);
    //		wl_surface_attach(display->cursor_surface, buffer, 0, 0);
    //		wl_surface_damage(display->cursor_surface, 0, 0,
    //				  image->width, image->height);
    //		wl_surface_commit(display->cursor_surface);
    //	}
    //}
    //
    //static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
    //		     uint32_t serial, struct wl_surface *surface)
    //{
    //}
    //
    //static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
    //		      uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
    //{
    //}
    //
    //static void pointer_handle_button(void *data, struct wl_pointer *wl_pointer,
    //		      uint32_t serial, uint32_t time, uint32_t button,
    //		      uint32_t state)
    //{
    //	struct display *display = data;
    //
    //	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
    //		wl_shell_surface_move(display->window->shell_surface,
    //				      display->seat, serial);
    //}
    //
    //static void pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
    //		    uint32_t time, uint32_t axis, wl_fixed_t value)
    //{
    //}
    //
    //struct wl_pointer_listener pointer_listener = {
    //	pointer_handle_enter,
    //	pointer_handle_leave,
    //	pointer_handle_motion,
    //	pointer_handle_button,
    //	pointer_handle_axis,
    //};
    
    static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
    		       uint32_t format, int fd, uint32_t size)
    {
    }
    
    static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
    		      uint32_t serial, struct wl_surface *surface,
    		      struct wl_array *keys)
    {
    }
    
    static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
    		      uint32_t serial, struct wl_surface *surface)
    {
    }
    
    static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
    		    uint32_t serial, uint32_t time, uint32_t key,
    		    uint32_t state)
    {
    	Seat *d = (Seat*)data;
    
      std::cerr << "key: " << key << std::endl;
    	//if (key == KEY_F11 && state)
    	//	toggle_fullscreen(d->window, d->window->fullscreen ^ 1);
    	//else if (key == KEY_ESC && state)
    	//	running = 0;
    }

    static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
    			  uint32_t serial, uint32_t mods_depressed,
    			  uint32_t mods_latched, uint32_t mods_locked,
    			  uint32_t group)
    {
    }
    
    static constexpr struct wl_keyboard_listener keyboard_listener = {
    	keyboard_handle_keymap,
    	keyboard_handle_enter,
    	keyboard_handle_leave,
    	keyboard_handle_key,
    	keyboard_handle_modifiers,
    };


    static void seat_handle_capabilities(void *data, struct wl_seat *seat,uint32_t capabilities){
      enum wl_seat_capability caps = (enum wl_seat_capability)capabilities;
    	Seat *d = (Seat*)data;
    
    	//if ((caps & WL_SEAT_CAPABILITY_POINTER) && !d->pointer) {
    	//	d->pointer = wl_seat_get_pointer(seat);
    	//	wl_pointer_add_listener(d->pointer, &Seat::pointer_listener, d);
    	//} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && d->pointer) {
    	//	wl_pointer_destroy(d->pointer);
    	//	d->pointer = NULL;
    	//}
    	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !d->keyboard) {
    		d->keyboard = wl_seat_get_keyboard(seat);
    		wl_keyboard_add_listener(d->keyboard, &keyboard_listener, d);
    	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && d->keyboard) {
    		wl_keyboard_destroy(d->keyboard);
    		d->keyboard = NULL;
    	}
    }

    struct wl_seat_listener seat_listener = {
	    seat_handle_capabilities,
    };
};

class wayland::Surface{
  public:
    Surface(Compositor const&c){
      auto d = [](struct wl_surface**p){wl_surface_destroy(*p);delete p;};
      surface = std::shared_ptr<struct wl_surface*>(new struct wl_surface*,d);
      auto cc = c.get();
      if(!cc)
        throw std::runtime_error("wayland::Surface::Surface() - compositor is empty");
      WAYLAND_CALLV(*surface,wl_compositor_create_surface,cc);
    }
    Surface(){}
    struct wl_surface*get()const{return *surface;}
  protected:
    std::shared_ptr<struct wl_surface*>surface;
};

class wayland::ShellSurface{
  public:
    ShellSurface(Shell const&shell,Surface const&surface){
      auto d = [](struct wl_shell_surface**p){wl_shell_surface_destroy(*p);delete p;};
      shellSurface = std::shared_ptr<struct wl_shell_surface*>(new struct wl_shell_surface*,d);
	    WAYLAND_CALLV(*shellSurface,wl_shell_get_shell_surface,shell.get(), surface.get());
	    wl_shell_surface_add_listener (*shellSurface, &shell_surface_listener, nullptr);
    }
    ShellSurface(){}
    void toggleFullscreen()const{
		  wl_shell_surface_set_fullscreen(*shellSurface,
		  				WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
		  				0, NULL);
    }
    void setTopLevel()const{
      wl_shell_surface_set_toplevel(*shellSurface);
    }
    struct wl_shell_surface*get()const{return *shellSurface;}
  protected:
    std::shared_ptr<struct wl_shell_surface*>shellSurface;

    static void shell_surface_ping (void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
    	wl_shell_surface_pong (shell_surface, serial);
    }
    static void shell_surface_configure (void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
      //struct window *window = (struct window*)data;
    	//wl_egl_window_resize (window->egl_window, width, height, 0, 0);
    }
    static void shell_surface_popup_done (void *data, struct wl_shell_surface *shell_surface) {
    	
    }
    struct wl_shell_surface_listener shell_surface_listener = {&shell_surface_ping, &shell_surface_configure, &shell_surface_popup_done};
};

wayland::Surface wayland::Compositor::getSurface()const{
  return Surface(*this);
}

class wayland::Registry{
  public:
    Registry(Display const&di){
      auto d = [](struct wl_registry**p){wl_registry_destroy(*p);delete p;};
      registry = std::shared_ptr<struct wl_registry*>(new struct wl_registry*,d);
      WAYLAND_CALLV(*registry,wl_display_get_registry,di.get());

      alloc(compositor);
      alloc(shell     );
      alloc(seat      );

      addListener();

      di.roundtrip();
    }
    Registry(){
    }
    Compositor getCompositor(){
      return Compositor(compositor);
    }
    Shell getShell(){
      return Shell(shell);
    }
    Seat getSeat(){
      return Seat(seat);
    }
  protected:
    template<typename T>
    void alloc(std::shared_ptr<T*>&a){
      auto d = [](T**p){delete p;};
      a = std::shared_ptr<T*>(new T*,d);
      *a = nullptr;
    }

    static void add_object(void*data,struct wl_registry*registry,uint32_t name,char const*interface,uint32_t version){
      auto _this = (Registry*)data;
      ___;
      std::cerr << "add_object: " << interface << std::endl;
	    if (!strcmp(interface,wl_compositor_interface.name))
	    	*_this->compositor = (wl_compositor*)wl_registry_bind (registry, name, &wl_compositor_interface, 1);
	    if (!strcmp(interface,wl_shell_interface.name))
    		*_this->shell = (wl_shell*)wl_registry_bind (registry, name, &wl_shell_interface, 1);
	    if (!strcmp(interface,wl_seat_interface.name)){
        *_this->seat = (wl_seat*)wl_registry_bind(registry,name,&wl_seat_interface,1);
      }

      ___;
    }
    static void remove_object (void *data, struct wl_registry *registry, uint32_t name) {
    }
    void addListener(){
	    WAYLAND_CALLR(wl_registry_add_listener,*registry, &registry_listener, this);
    }
    static constexpr struct wl_registry_listener registry_listener = {
      &add_object   ,
      &remove_object,
    };
    std::shared_ptr<struct wl_registry  *>registry  ;
    std::shared_ptr<struct wl_compositor*>compositor;
    std::shared_ptr<struct wl_shell     *>shell     ;
    std::shared_ptr<struct wl_seat      *>seat      ;

};

wayland::Registry wayland::Display::getRegistry()const{
  return Registry(*this);
}

