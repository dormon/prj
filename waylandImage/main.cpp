#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <memory>
#include <vector>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>

//#include "helpers.h"

namespace wayland{
  class Window;
  class Display;
  class Registry;
};

namespace egl{
  class Display;
  class Context;
};

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

class wayland::Registry{
  public:
    Registry(Display const&di){
      auto d = [](struct wl_registry**p){wl_registry_destroy(*p);delete p;};
      registry = std::shared_ptr<struct wl_registry*>(new struct wl_registry*,d);
      *registry = wl_display_get_registry(di.get());
    }
  protected:
    std::shared_ptr<struct wl_registry*>registry;
};

wayland::Registry wayland::Display::getRegistry()const{
  return Registry(*this);
}

class egl::Display{
  public:
    Display(wayland::Display const&di){
      auto d = [](EGLDisplay*p){eglTerminate(*p);delete p;};
      display = std::shared_ptr<EGLDisplay>(new EGLDisplay,d);
      *display = eglGetDisplay(di.get());
      if(*display == EGL_NO_DISPLAY)
        throw std::runtime_error("eglGetDisplay failed");
      auto err = eglInitialize(*display,nullptr,nullptr);
      if(err != EGL_TRUE)
        throw std::runtime_error("eglInitialize failed");
    }
    EGLDisplay get()const{
      return *display;
    }
  protected:
    std::shared_ptr<EGLDisplay>display;
};

egl::Display wayland::Display::getEGLDisplay()const{
  return egl::Display(*this);
}

class egl::Context{
  public:
    Context(egl::Display const&d){
      if(eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)
        throw std::runtime_error("eglBindAPI(EGL_OPENGL_ES_API) failed");
      EGLConfig config;
      EGLint nofConfig;
      eglChooseConfig(d.get(),attributes.data(),&config,1,&nofConfig);
      eglCreateContext(d.get(),config,EGL_NO_CONTEXT,nullptr);
          
    }
  protected:
    std::vector<EGLint>attributes = {
      EGL_RED_SIZE,8,
      EGL_GREEN_SIZE,8,
      EGL_BLUE_SIZE,8,
      EGL_NONE};
};

class wayland::Window{
  public:
    Window(){
      display = wl_display_connect(nullptr);
    }
    ~Window(){
	    wl_display_disconnect(display);
    }
  protected:
    struct wl_display*display;

};

static const unsigned WIDTH = 320;
static const unsigned HEIGHT = 200;

static bool done = false;

void on_button(uint32_t button)
{
    done = true;
}

int main(void)
{
  auto display = wayland::Display();
  /*
    struct wl_buffer *buffer;
    struct wl_shm_pool *pool;
    struct wl_shell_surface *surface;
    int image;

    hello_setup_wayland();

    image = open("images.bin", O_RDWR);

    if (image < 0) {
        perror("Error opening surface image");
        return EXIT_FAILURE;
    }

    pool = hello_create_memory_pool(image);
    surface = hello_create_surface();
    buffer = hello_create_buffer(pool, WIDTH, HEIGHT);
    hello_bind_buffer(buffer, surface);

    while (!done) {
        if (wl_display_dispatch(display) < 0) {
            perror("Main loop error");
            done = true;
        }
    }

    fprintf(stderr, "Exiting sample wayland client...\n");

    hello_free_buffer(buffer);
    hello_free_surface(surface);
    hello_free_memory_pool(pool);
    close(image);

    hello_cleanup_wayland();
*/
    return EXIT_SUCCESS;
}
