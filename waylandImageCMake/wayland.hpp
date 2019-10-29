#pragma once

#include <memory>
#include <iostream>
#include <functional>

#include <string.h>

#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>

#include "util.h"
#define ENABLE_PRINT_CALL_STACK
#include "callstackPrinter.h"

namespace wayland{
  class Display;
  class Registry;
  class Compositor;
  class Shell;
  class Seat;
  class SeatListener;
  class Surface;
  class EGLWindow;
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
    Display(char const*name = nullptr);
    Registry getRegistry()const;
    int roundtrip()const;
    struct wl_display*get()const;
  protected:
    std::shared_ptr<struct wl_display*>display;
};

class wayland::Compositor{
  public:
    Compositor(std::shared_ptr<struct wl_compositor*>c);
    Compositor();
    struct wl_compositor* get()const;
    bool isReady()const;
    Surface getSurface()const;
  protected:
    std::shared_ptr<struct wl_compositor*>compositor;
};

class wayland::Shell{
  public:
    Shell(std::shared_ptr<struct wl_shell*>const&s);
    Shell();
    struct wl_shell* get()const;
  protected:
    std::shared_ptr<struct wl_shell*>shell;
};

class wayland::SeatListener{
  public:
    //struct wl_pointer *pointer;
	  struct wl_keyboard *keyboard = nullptr;
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
      std::cerr << __func__ << std::endl;
    }
    
    static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
    		      uint32_t serial, struct wl_surface *surface,
    		      struct wl_array *keys)
    {
      std::cerr << __func__ << std::endl;
    }
    
    static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
    		      uint32_t serial, struct wl_surface *surface)
    {
      std::cerr << __func__ << std::endl;
    }
    
    static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
    		    uint32_t serial, uint32_t time, uint32_t key,
    		    uint32_t state)
    {
      std::cerr << __func__ << std::endl;
      std::cerr << "key: " << key << std::endl;
      std::cerr << "state: " << state << std::endl;
    	SeatListener *d = (SeatListener*)data;
      if(state == 1 && d->onKeyDown)d->onKeyDown(key);
      //exit(0);
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
      std::cerr << __func__ << std::endl;
    }
    
    static constexpr struct wl_keyboard_listener keyboard_listener = {
    	keyboard_handle_keymap,
    	keyboard_handle_enter,
    	keyboard_handle_leave,
    	keyboard_handle_key,
    	keyboard_handle_modifiers,
    };


    static void seat_handle_capabilities(void *data, struct wl_seat *seat,uint32_t capabilities){
      PRINT_CALL_STACK(data,seat,capabilities);
      enum wl_seat_capability caps = (enum wl_seat_capability)capabilities;
    	SeatListener *d = (SeatListener*)data;
      std::cerr << "seat_handle_capabilities" << std::endl;
      ___;
    	//if ((caps & WL_SEAT_CAPABILITY_POINTER) && !d->pointer) {
    	//	d->pointer = wl_seat_get_pointer(seat);
    	//	wl_pointer_add_listener(d->pointer, &Seat::pointer_listener, d);
    	//} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && d->pointer) {
    	//	wl_pointer_destroy(d->pointer);
    	//	d->pointer = NULL;
    	//}
      std::cerr << "caps: " << caps << "  keyboard: " << d->keyboard << std::endl;
    	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !d->keyboard) {
        ___;
    		d->keyboard = wl_seat_get_keyboard(seat);
    		wl_keyboard_add_listener(d->keyboard, &keyboard_listener, d);
    	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && d->keyboard) {
        ___;
    		wl_keyboard_destroy(d->keyboard);
    		d->keyboard = NULL;
    	}
      ___;
    }

    struct wl_seat_listener static constexpr  seat_listener = {
	    seat_handle_capabilities,
    };

    std::function<void(uint32_t)>onKeyDown;

};

class wayland::Seat{
  public:
    Seat(std::shared_ptr<struct wl_seat*>const&s):seat(s){seatListener = std::make_shared<SeatListener>();}
    Seat(){}
    auto get()const{
      return *seat;
    }
    void addListener(){
      WAYLAND_CALLR(wl_seat_add_listener,*seat, &SeatListener::seat_listener,&*seatListener);
    }
    void setOnKeyDown(std::function<void(uint32_t)>const&fce){
      seatListener->onKeyDown = fce;
    }
    std::shared_ptr<struct wl_seat*>seat;
    std::shared_ptr<SeatListener>seatListener;

};

class wayland::EGLWindow{
  public:
    EGLWindow(struct wl_surface* waylandSurface,uint32_t width,uint32_t height);
    EGLWindow();
    struct wl_egl_window* get()const;
  protected:
    std::shared_ptr<struct wl_egl_window*>window;
};

class wayland::Surface{
  public:
    Surface(Compositor const&c);
    Surface();
    struct wl_surface*get()const;
    EGLWindow getEGLWindow(uint32_t width,uint32_t height)const;
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

// DISPLAY IMPL

wayland::Display::Display(char const*name){
  auto d = [](struct wl_display**p){wl_display_disconnect(*p);delete p;};
  display = std::shared_ptr<struct wl_display*>(new struct wl_display*,d);
  WAYLAND_CALLV(*display,wl_display_connect,name);
}

wayland::Registry wayland::Display::getRegistry()const{
  return Registry(*this);
}

int wayland::Display::roundtrip()const{
  return wl_display_roundtrip(*display);
}

struct wl_display*wayland::Display::get()const{return *display;}

// DISPLAY IMPL END





// COMPOSITOR IMPL

wayland::Compositor::Compositor(std::shared_ptr<struct wl_compositor*>c):compositor(c){}

wayland::Compositor::Compositor(){}

struct wl_compositor* wayland::Compositor::get()const{
  return *compositor;
}

bool wayland::Compositor::isReady()const{
  return compositor != nullptr && *compositor != nullptr;
}

wayland::Surface wayland::Compositor::getSurface()const{
  return Surface(*this);
}

// COMPOSITOR IMPL END





// SHELL IMPL

wayland::Shell::Shell(std::shared_ptr<struct wl_shell*>const&s):shell(s){}

wayland::Shell::Shell(){}

struct wl_shell* wayland::Shell::get()const{
  return *shell;
}

// SHELL IMPL END






// EGLWindow IMPL

wayland::EGLWindow::EGLWindow(struct wl_surface* waylandSurface,uint32_t width,uint32_t height){
  auto d = [](struct wl_egl_window**p){wl_egl_window_destroy(*p);delete p;};
  window = std::shared_ptr<struct wl_egl_window*>(new struct wl_egl_window*,d);
  WAYLAND_CALLV(*window,wl_egl_window_create,waylandSurface, width, height);
}

wayland::EGLWindow::EGLWindow(){}

struct wl_egl_window* wayland::EGLWindow::get()const{return*window;}

// EGLWindow IMPL END





// Surface IMPL

wayland::Surface::Surface(Compositor const&c){
  auto d = [](struct wl_surface**p){wl_surface_destroy(*p);delete p;};
  surface = std::shared_ptr<struct wl_surface*>(new struct wl_surface*,d);
  auto cc = c.get();
  if(!cc)
    throw std::runtime_error("wayland::Surface::Surface() - compositor is empty");
  WAYLAND_CALLV(*surface,wl_compositor_create_surface,cc);
}

wayland::Surface::Surface(){}

struct wl_surface*wayland::Surface::get()const{return *surface;}

wayland::EGLWindow wayland::Surface::getEGLWindow(uint32_t width,uint32_t height)const{
  return EGLWindow(*surface,width,height);
}

// Surface IMPL END
