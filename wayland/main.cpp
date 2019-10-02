// gcc -o wayland-egl wayland-egl.c -lwayland-client -lwayland-egl -lEGL -lGL

#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#include <string.h>
#include <iostream>

#define WIDTH 256
#define HEIGHT 256

#define ___ std::cerr << __FILE__ << " : " << __LINE__ << std::endl

static struct wl_display *display;
static struct wl_compositor *compositor = NULL;
static struct wl_shell *shell = NULL;
static EGLDisplay egl_display;
static char running = 1;

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

static void create_window (struct window *window, int32_t width, int32_t height) {
  ___;
	eglBindAPI (EGL_OPENGL_API);//
  ___;
	EGLint attributes[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
	EGL_NONE};
	EGLConfig config;
	EGLint num_config;
  ___;
	eglChooseConfig (egl_display, attributes, &config, 1, &num_config);//
  ___;
	window->egl_context = eglCreateContext (egl_display, config, EGL_NO_CONTEXT, NULL);//
  ___;
	
	window->surface = wl_compositor_create_surface (compositor);//
  ___;
	window->shell_surface = wl_shell_get_shell_surface (shell, window->surface);//
  ___;
	wl_shell_surface_add_listener (window->shell_surface, &shell_surface_listener, window);//
  ___;
	wl_shell_surface_set_toplevel (window->shell_surface);//
  ___;
	window->egl_window = wl_egl_window_create (window->surface, width, height);//
  ___;
	window->egl_surface = eglCreateWindowSurface (egl_display, config, window->egl_window, NULL);//
  ___;
	eglMakeCurrent (egl_display, window->egl_surface, window->egl_surface, window->egl_context);//
  ___;
}
static void delete_window (struct window *window) {
  ___;
	eglDestroySurface (egl_display, window->egl_surface);
  ___;
	wl_egl_window_destroy (window->egl_window);
  ___;
	wl_shell_surface_destroy (window->shell_surface);
  ___;
	wl_surface_destroy (window->surface);
  ___;
	eglDestroyContext (egl_display, window->egl_context);
  ___;
}
static void draw_window (struct window *window) {
  ___;
	glClearColor (0.0, 1.0, 0.0, 1.0);
  ___;
	glClear (GL_COLOR_BUFFER_BIT);
  ___;
	eglSwapBuffers (egl_display, window->egl_surface);
  ___;
}

int main () {
  ___;
	display = wl_display_connect (NULL);//
  ___;
	struct wl_registry *registry = wl_display_get_registry (display);//
  ___;
	wl_registry_add_listener (registry, &registry_listener, NULL);//
  ___;
	wl_display_roundtrip (display);//
  ___;
	
	egl_display = eglGetDisplay (display);//
  ___;
	eglInitialize (egl_display, NULL, NULL);//
  ___;
	
	struct window window;
  ___;
	create_window (&window, WIDTH, HEIGHT);//
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
