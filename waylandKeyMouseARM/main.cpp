#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <signal.h>
#include <iostream>

#include <linux/input.h>

#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

struct window;
struct seat;

struct display {
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_shell *shell;
	struct wl_seat *seat;
	struct wl_pointer *pointer;
	struct wl_keyboard *keyboard;
	struct wl_shm *shm;
	struct wl_cursor_theme *cursor_theme;
	struct wl_cursor *default_cursor;
	struct wl_surface *cursor_surface;
	struct {
		EGLDisplay dpy;
		EGLContext ctx;
		EGLConfig conf;
	} egl;
	struct window *window;
};

struct geometry {
	int width, height;
};

struct window {
	struct display *display;
	struct geometry geometry, window_size;
	struct {
		GLuint rotation_uniform;
		GLuint pos;
		GLuint col;
	} gl;

	struct wl_egl_window *native;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	EGLSurface egl_surface;
	struct wl_callback *callback;
	int fullscreen, configured, opaque;
};

static int running = 1;

#define CALL_STACK() std::cerr << __func__ << " " << __LINE__ << std::endl

#define ENABLE_PRINT_CALL_STACK
#include "callstackPrinter.h"


static void
init_egl(struct display *display, int opaque)
{
  PRINT_CALL_STACK(display,opaque);
  CALL_STACK();
	static const EGLint context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
  CALL_STACK();

	EGLint config_attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_ALPHA_SIZE, 1,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};
  CALL_STACK();

	EGLint major, minor, n;
	EGLBoolean ret;

	if (opaque)
		config_attribs[9] = 0;

	display->egl.dpy = eglGetDisplay(display->display);
	assert(display->egl.dpy);
  CALL_STACK();

	ret = eglInitialize(display->egl.dpy, &major, &minor);
	assert(ret == EGL_TRUE);
	ret = eglBindAPI(EGL_OPENGL_ES_API);
	assert(ret == EGL_TRUE);
  CALL_STACK();

	ret = eglChooseConfig(display->egl.dpy, config_attribs,
			      &display->egl.conf, 1, &n);
	assert(ret && n == 1);
  CALL_STACK();

	display->egl.ctx = eglCreateContext(display->egl.dpy,
					    display->egl.conf,
					    EGL_NO_CONTEXT, context_attribs);
	assert(display->egl.ctx);
  CALL_STACK();

}

static void
fini_egl(struct display *display)
{
  PRINT_CALL_STACK(display);
	/* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
	 * on eglReleaseThread(). */
	eglMakeCurrent(display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE,
		       EGL_NO_CONTEXT);

	eglTerminate(display->egl.dpy);
	eglReleaseThread();
}


static void
handle_ping(void *data, struct wl_shell_surface *shell_surface,
	    uint32_t serial)
{
  PRINT_CALL_STACK(data,shell_surface,serial);
	wl_shell_surface_pong(shell_surface, serial);
}

static void
handle_configure(void *data, struct wl_shell_surface *shell_surface,
		 uint32_t edges, int32_t width, int32_t height)
{
  PRINT_CALL_STACK(data,shell_surface,edges,width,height);
	struct window *window = (struct window*)data;

	if (window->native)
		wl_egl_window_resize(window->native, width, height, 0, 0);

	window->geometry.width = width;
	window->geometry.height = height;

	if (!window->fullscreen)
		window->window_size = window->geometry;
}

static void
handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
  PRINT_CALL_STACK(data,shell_surface);
}

static const struct wl_shell_surface_listener shell_surface_listener = {
	handle_ping,
	handle_configure,
	handle_popup_done
};

static void
redraw(void *data, struct wl_callback *callback, uint32_t time);

static void
configure_callback(void *data, struct wl_callback *callback, uint32_t  time)
{
  PRINT_CALL_STACK(data,callback,time);
	struct window *window = (struct window*)data;

	wl_callback_destroy(callback);

	window->configured = 1;

	if (window->callback == NULL)
		redraw(data, NULL, time);
}

static struct wl_callback_listener configure_callback_listener = {
	configure_callback,
};

static void
toggle_fullscreen(struct window *window, int fullscreen)
{
  PRINT_CALL_STACK(window,fullscreen);
	struct wl_callback *callback;

	window->fullscreen = fullscreen;
	window->configured = 0;

	if (fullscreen) {
		wl_shell_surface_set_fullscreen(window->shell_surface,
						WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
						0, NULL);
	} else {
		wl_shell_surface_set_toplevel(window->shell_surface);
		handle_configure(window, window->shell_surface, 0,
				 window->window_size.width,
				 window->window_size.height);
	}

	callback = wl_display_sync(window->display->display);
	wl_callback_add_listener(callback, &configure_callback_listener,
				 window);
}

static void
create_surface(struct window *window)
{
  PRINT_CALL_STACK(window);
	struct display *display = window->display;
	EGLBoolean ret;
	
	window->surface = wl_compositor_create_surface(display->compositor);
	window->shell_surface = wl_shell_get_shell_surface(display->shell,
							   window->surface);

	wl_shell_surface_add_listener(window->shell_surface,
				      &shell_surface_listener, window);

	window->native =
		wl_egl_window_create(window->surface,
				     window->window_size.width,
				     window->window_size.height);
	window->egl_surface =
		eglCreateWindowSurface(display->egl.dpy,
				       display->egl.conf,
				       window->native, NULL);

	wl_shell_surface_set_title(window->shell_surface, "simple-egl");

	ret = eglMakeCurrent(window->display->egl.dpy, window->egl_surface,
			     window->egl_surface, window->display->egl.ctx);
	assert(ret == EGL_TRUE);

	toggle_fullscreen(window, window->fullscreen);
}

static void
destroy_surface(struct window *window)
{
  PRINT_CALL_STACK(window);
	wl_egl_window_destroy(window->native);

	wl_shell_surface_destroy(window->shell_surface);
	wl_surface_destroy(window->surface);

	if (window->callback)
		wl_callback_destroy(window->callback);
}

//static const struct wl_callback_listener frame_listener;

static void
redraw(void *data, struct wl_callback *callback, uint32_t time);

static const struct wl_callback_listener frame_listener = {
	redraw
};


static void
redraw(void *data, struct wl_callback *callback, uint32_t time)
{
  //CALL_STACK();
	struct window *window = (struct window*)data;
	static const int32_t speed_div = 5;
	static uint32_t start_time = 0;
	struct wl_region *region;

	assert(window->callback == callback);
	window->callback = NULL;

	if (callback)
		wl_callback_destroy(callback);

	if (!window->configured)
		return;

	if (start_time == 0)
		start_time = time;

	if (window->opaque || window->fullscreen) {
		region = wl_compositor_create_region(window->display->compositor);
		wl_region_add(region, 0, 0,
			      window->geometry.width,
			      window->geometry.height);
		wl_surface_set_opaque_region(window->surface, region);
		wl_region_destroy(region);
	} else {
		wl_surface_set_opaque_region(window->surface, NULL);
	}

	window->callback = wl_surface_frame(window->surface);
	wl_callback_add_listener(window->callback, &frame_listener, window);

	eglSwapBuffers(window->display->egl.dpy, window->egl_surface);
}

static void
pointer_handle_enter(void *data, struct wl_pointer *pointer,
		     uint32_t serial, struct wl_surface *surface,
		     wl_fixed_t sx, wl_fixed_t sy)
{
  PRINT_CALL_STACK(data,pointer,serial,surface,sx,sy);
	struct display *display = (struct display*)data;
	struct wl_buffer *buffer;
	struct wl_cursor *cursor = display->default_cursor;
	struct wl_cursor_image *image;

	if (display->window->fullscreen)
		wl_pointer_set_cursor(pointer, serial, NULL, 0, 0);
	else if (cursor) {
		image = display->default_cursor->images[0];
		buffer = wl_cursor_image_get_buffer(image);
		wl_pointer_set_cursor(pointer, serial,
				      display->cursor_surface,
				      image->hotspot_x,
				      image->hotspot_y);
		wl_surface_attach(display->cursor_surface, buffer, 0, 0);
		wl_surface_damage(display->cursor_surface, 0, 0,
				  image->width, image->height);
		wl_surface_commit(display->cursor_surface);
	}
}

static void
pointer_handle_leave(void *data, struct wl_pointer *pointer,
		     uint32_t serial, struct wl_surface *surface)
{
  PRINT_CALL_STACK(data,pointer,serial,surface);
}

static void
pointer_handle_motion(void *data, struct wl_pointer *pointer,
		      uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
  PRINT_CALL_STACK(data,pointer,time,sx,sy);
}

static void
pointer_handle_button(void *data, struct wl_pointer *wl_pointer,
		      uint32_t serial, uint32_t time, uint32_t button,
		      uint32_t state)
{
  PRINT_CALL_STACK(data,wl_pointer,serial,time,button,state);
	struct display *display = (struct display*)data;

	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
		wl_shell_surface_move(display->window->shell_surface,
				      display->seat, serial);
}

static void
pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
		    uint32_t time, uint32_t axis, wl_fixed_t value)
{
  PRINT_CALL_STACK(data,wl_pointer,time,axis,value);
}

static const struct wl_pointer_listener pointer_listener = {
	pointer_handle_enter,
	pointer_handle_leave,
	pointer_handle_motion,
	pointer_handle_button,
	pointer_handle_axis,
};

static void
keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
		       uint32_t format, int fd, uint32_t size)
{
  PRINT_CALL_STACK(data,keyboard,format,fd,size);
}

static void
keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
		      uint32_t serial, struct wl_surface *surface,
		      struct wl_array *keys)
{
  PRINT_CALL_STACK(data,keyboard,serial,surface,keys);
}

static void
keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
		      uint32_t serial, struct wl_surface *surface)
{
  PRINT_CALL_STACK(data,keyboard,serial,surface);
}

static void
keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
		    uint32_t serial, uint32_t time, uint32_t key,
		    uint32_t state)
{
  PRINT_CALL_STACK(data,keyboard,serial,time,key,state);
	struct display *d = (struct display*)data;

	if (key == KEY_F11 && state)
		toggle_fullscreen(d->window, d->window->fullscreen ^ 1);
	else if (key == KEY_ESC && state)
		running = 0;
}

static void
keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
			  uint32_t serial, uint32_t mods_depressed,
			  uint32_t mods_latched, uint32_t mods_locked,
			  uint32_t group)
{
  PRINT_CALL_STACK(data,keyboard,serial,mods_depressed,mods_latched,mods_locked,group);
}

static const struct wl_keyboard_listener keyboard_listener = {
	keyboard_handle_keymap,
	keyboard_handle_enter,
	keyboard_handle_leave,
	keyboard_handle_key,
	keyboard_handle_modifiers,
};

static void
seat_handle_capabilities(void *data, struct wl_seat *seat,uint32_t capabilities)
//			 enum wl_seat_capability caps)
{
  PRINT_CALL_STACK(data,seat,capabilities);
  enum wl_seat_capability caps = (wl_seat_capability)capabilities;
	struct display *d = (struct display*)data;

	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !d->pointer) {
    CALL_STACK();
		d->pointer = wl_seat_get_pointer(seat);
		wl_pointer_add_listener(d->pointer, &pointer_listener, d);
	} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && d->pointer) {
    CALL_STACK();
		wl_pointer_destroy(d->pointer);
		d->pointer = NULL;
	}

  std::cerr << "caps: " << caps << "  keyboard: " << d->keyboard << std::endl; 
	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !d->keyboard) {
    CALL_STACK();
		d->keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(d->keyboard, &keyboard_listener, d);
	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && d->keyboard) {
    CALL_STACK();
		wl_keyboard_destroy(d->keyboard);
		d->keyboard = NULL;
	}
  CALL_STACK();
}

static const struct wl_seat_listener seat_listener = {
	seat_handle_capabilities,
};

static void
registry_handle_global(void *data, struct wl_registry *registry,
		       uint32_t name, const char *interface, uint32_t version)
{
  PRINT_CALL_STACK(data,registry,name,interface,version);
	struct display *d = (struct display*)data;

	if (strcmp(interface, "wl_compositor") == 0) {
		d->compositor = (wl_compositor*)
			wl_registry_bind(registry, name,
					 &wl_compositor_interface, 1);
	} else if (strcmp(interface, "wl_shell") == 0) {
		d->shell = (wl_shell*)wl_registry_bind(registry, name,
					    &wl_shell_interface, 1);
	} else if (strcmp(interface, "wl_seat") == 0) {
		d->seat = (wl_seat*)wl_registry_bind(registry, name,
					   &wl_seat_interface, 1);
		wl_seat_add_listener(d->seat, &seat_listener, d);
	} else if (strcmp(interface, "wl_shm") == 0) {
		d->shm = (wl_shm*)wl_registry_bind(registry, name,
					  &wl_shm_interface, 1);
		d->cursor_theme = wl_cursor_theme_load(NULL, 32, d->shm);
		d->default_cursor =
			wl_cursor_theme_get_cursor(d->cursor_theme, "left_ptr");
	}
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
			      uint32_t name)
{
  PRINT_CALL_STACK(data,registry,name);
}

static const struct wl_registry_listener registry_listener = {
	registry_handle_global,
	registry_handle_global_remove
};

static void
signal_int(int signum)
{
  PRINT_CALL_STACK(signum);
	running = 0;
}

static void
usage(int error_code)
{
  PRINT_CALL_STACK(error_code);
	fprintf(stderr, "Usage: simple-egl [OPTIONS]\n\n"
		"  -f\tRun in fullscreen mode\n"
		"  -o\tCreate an opaque surface\n"
		"  -h\tThis help text\n\n");

	exit(error_code);
}

int
main(int argc, char **argv)
{
  PRINT_CALL_STACK(argc,argv);
	struct sigaction sigint;
	struct display display = { 0 };
	struct window  window  = { 0 };
	int i, ret = 0;
  CALL_STACK();

	window.display = &display;
	display.window = &window;
	window.window_size.width  = 250;
	window.window_size.height = 250;

	for (i = 1; i < argc; i++) {
		if (strcmp("-f", argv[i]) == 0)
			window.fullscreen = 1;
		else if (strcmp("-o", argv[i]) == 0)
			window.opaque = 1;
		else if (strcmp("-h", argv[i]) == 0)
			usage(EXIT_SUCCESS);
		else
			usage(EXIT_FAILURE);
	}
  CALL_STACK();

	display.display = wl_display_connect(NULL);
	assert(display.display);
  CALL_STACK();

	display.registry = wl_display_get_registry(display.display);
	wl_registry_add_listener(display.registry,
				 &registry_listener, &display);
  CALL_STACK();

	wl_display_dispatch(display.display);
  CALL_STACK();

	init_egl(&display, window.opaque);
  CALL_STACK();
	create_surface(&window);
  CALL_STACK();

	display.cursor_surface =
		wl_compositor_create_surface(display.compositor);
  CALL_STACK();

	sigint.sa_handler = signal_int;
	sigemptyset(&sigint.sa_mask);
	sigint.sa_flags = SA_RESETHAND;
	sigaction(SIGINT, &sigint, NULL);
  CALL_STACK();

	while (running && ret != -1){
    CALL_STACK();
		ret = wl_display_dispatch(display.display);
    CALL_STACK();
  }
  CALL_STACK();

	fprintf(stderr, "simple-egl exiting\n");

	destroy_surface(&window);
	fini_egl(&display);

	wl_surface_destroy(display.cursor_surface);
	if (display.cursor_theme)
		wl_cursor_theme_destroy(display.cursor_theme);

	if (display.shell)
		wl_shell_destroy(display.shell);

	if (display.compositor)
		wl_compositor_destroy(display.compositor);

	wl_registry_destroy(display.registry);
	wl_display_flush(display.display);
	wl_display_disconnect(display.display);

	return 0;
}
