// gcc -o wayland-egl wayland-egl.c -lwayland-client -lwayland-egl -lEGL -lGL

#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#include <string.h>
#include <iostream>

#define WIDTH (1920*2)
#define HEIGHT (1080*2)

#define ___ std::cerr << __FILE__ << " : " << __LINE__ << std::endl

static struct wl_display *display;
static struct wl_compositor *compositor = NULL;
static struct wl_shell *shell = NULL;
static EGLDisplay egl_display;
static char running = 1;


decltype(glClear)* ptr2;
decltype(glClearColor)* ptr3;

struct window {
	EGLContext egl_context;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	struct wl_egl_window *egl_window;
	EGLSurface egl_surface;
};

// listeners
static void registry_add_object (void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
  ___;
	if (!strcmp(interface,"wl_compositor")) {
		compositor = (wl_compositor*)wl_registry_bind (registry, name, &wl_compositor_interface, 1);
	}
	else if (!strcmp(interface,"wl_shell")) {
		shell = (wl_shell*)wl_registry_bind (registry, name, &wl_shell_interface, 1);
	}
  ___;
}
static void registry_remove_object (void *data, struct wl_registry *registry, uint32_t name) {
	
}
static struct wl_registry_listener registry_listener = {&registry_add_object, &registry_remove_object};

static void shell_surface_ping (void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
  ___;
	wl_shell_surface_pong (shell_surface, serial);
  ___;
}
static void shell_surface_configure (void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
  ___;
	struct window *window = (struct window*)data;
	wl_egl_window_resize (window->egl_window, width, height, 0, 0);
  ___;
}
static void shell_surface_popup_done (void *data, struct wl_shell_surface *shell_surface) {
	
}
static struct wl_shell_surface_listener shell_surface_listener = {&shell_surface_ping, &shell_surface_configure, &shell_surface_popup_done};

std::string translateError(EGLint err){
  switch(err){
    case EGL_SUCCESS:
      return "The last function succeeded without error.";
    case EGL_NOT_INITIALIZED:
      return "EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
    case EGL_BAD_ACCESS:
      return "EGL cannot access a requested resource (for example a context is bound in another thread).";
    case EGL_BAD_ALLOC:
      return "EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE:
      return "An unrecognized attribute or attribute value was passed in the attribute list.";
    case EGL_BAD_CONTEXT:
      return "An EGLContext argument does not name a valid EGL rendering context.";
    case EGL_BAD_CONFIG:
      return "An EGLConfig argument does not name a valid EGL frame buffer configuration.";
    case EGL_BAD_CURRENT_SURFACE:
      return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY:
      return "An EGLDisplay argument does not name a valid EGL display connection.";
    case EGL_BAD_SURFACE:
      return "An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH:
      return "Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER:
      return "One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP:
      return "A NativePixmapType argument does not refer to a valid native pixmap.";
    case EGL_BAD_NATIVE_WINDOW:
      return "A NativeWindowType argument does not refer to a valid native window.";
    case EGL_CONTEXT_LOST:
      return "A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
  }
  return "";
}



static void create_window (struct window *window, int32_t width, int32_t height) {
  ___;
	if(eglBindAPI (EGL_OPENGL_ES_API) != EGL_TRUE){
    std::cerr << "eglBindAPI failed" << std::endl;
    exit(0);
  }
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
	EGLint attributes[] = {
    //EGL_SAMPLES,0,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
    //EGL_ALPHA_SIZE,0,
    //EGL_DEPTH_SIZE,24,
    //EGL_RENDERABLE_TYPE,EGL_OPENGL_ES_BIT|EGL_OPENGL_ES2_BIT|EGL_OPENGL_ES3_BIT,
    //EGL_SURFACE_TYPE,EGL_PBUFFER_BIT|EGL_PIXMAP_BIT|EGL_SWAP_BEHAVIOR_PRESERVED_BIT|EGL_WINDOW_BIT|0x180,
	EGL_NONE};
	EGLConfig config;
	EGLint num_config;
  ___;
	if(eglChooseConfig (egl_display, attributes, &config, 1, &num_config) !=  EGL_TRUE){
    std::cerr << "eglChooseConfig failed" << std::endl;
    exit(0);
  }
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
  EGLint aatt[]={
  EGL_CONTEXT_MAJOR_VERSION,
  3,
  EGL_CONTEXT_MINOR_VERSION,
  1,
  //EGL_CONTEXT_OPENGL_DEBUG,
  //EGL_TRUE,
  EGL_NONE,
  };

	window->egl_context = eglCreateContext (egl_display, config, EGL_NO_CONTEXT, aatt);
  if(window->egl_context ==  EGL_NO_CONTEXT){
    std::cerr << "eglCreateContext failed - " << translateError(eglGetError()) << std::endl;
    exit(0);
  } 
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;

  ptr2 = (decltype(glClear)*)eglGetProcAddress("glClear");
  ptr3 = (decltype(glClearColor)*)eglGetProcAddress("glClearColor");
  ___;
	
	window->surface = wl_compositor_create_surface (compositor);
  ___;
	window->shell_surface = wl_shell_get_shell_surface (shell, window->surface);
  wl_shell_surface_set_fullscreen(window->shell_surface,WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,0,nullptr);
  ___;
	wl_shell_surface_add_listener (window->shell_surface, &shell_surface_listener, window);
  ___;
	wl_shell_surface_set_toplevel (window->shell_surface);
  ___;
	window->egl_window = wl_egl_window_create (window->surface, width, height);
  ___;
	window->egl_surface = eglCreateWindowSurface (egl_display, config, window->egl_window, NULL);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
	eglMakeCurrent (egl_display, window->egl_surface, window->egl_surface, window->egl_context);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
}
static void delete_window (struct window *window) {
  ___;
	eglDestroySurface (egl_display, window->egl_surface);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
	wl_egl_window_destroy (window->egl_window);
  ___;
	wl_shell_surface_destroy (window->shell_surface);
  ___;
	wl_surface_destroy (window->surface);
  ___;
	eglDestroyContext (egl_display, window->egl_context);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
}
static void draw_window (struct window *window) {
  ___;
  //glGetError();
  //auto ptr = (decltype(glGetError)*)eglGetProcAddress("glGetError");
  //auto a = ptr();
  //std::cerr << "glGetError() - " << a << std::endl;
  ptr3(1.f,1.f,1.f,1.f);
  ptr2(GL_COLOR_BUFFER_BIT);
  //std::cerr << "ahoj" << std::endl;
  ////((decltype(glGetError)*)eglGetProcAddress("glGetError"))();
	//glClearColor (0.0, 1.0, 0.0, 1.0);
  ___;
	//glClear (GL_COLOR_BUFFER_BIT);
  ___;
	eglSwapBuffers (egl_display, window->egl_surface);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
}

int main () {
  ___;
	display = wl_display_connect (NULL);
  ___;
	struct wl_registry *registry = wl_display_get_registry (display);
  ___;
	wl_registry_add_listener (registry, &registry_listener, NULL);
  ___;
	wl_display_roundtrip (display);
  ___;
	
	egl_display = eglGetDisplay (display);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
	eglInitialize (egl_display, NULL, NULL);
  std::cerr << __LINE__ << " - " << translateError(eglGetError()) << std::endl;
  ___;
	
	struct window window;
  ___;
	create_window (&window, WIDTH, HEIGHT);
  ___;
	
	while (running) {
    ___;
		wl_display_dispatch_pending (display);
    ___;
		draw_window (&window);
    ___;
	}
	
  ___;
	delete_window (&window);
  ___;
	eglTerminate (egl_display);
  ___;
	wl_display_disconnect (display);
  ___;
	return 0;
}
