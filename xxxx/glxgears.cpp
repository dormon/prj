/*
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/* $XFree86: xc/programs/glxgears/glxgears.c,v 1.2 2001/04/03 15:56:26 dawes Exp
 * $ */

/*
 * This is a port of the infamous "gears" demo to straight GLX (i.e. no GLUT)
 * Port by Brian Paul  23 March 2001
 *
 * Command line options:
 *    -info      print GL implementation information
 *
 */

#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/extensions/XInput.h>

#define BENCHMARK

#ifdef BENCHMARK

/* XXX this probably isn't very portable */

#include <sys/time.h>
#include <unistd.h>

/* return current time (in seconds) */
static int current_time(void)
{
  struct timeval  tv;
  struct timezone tz;
  (void)gettimeofday(&tv, &tz);
  return (int)tv.tv_sec;
}

#else /*BENCHMARK*/

/* dummy */
static int current_time(void) { return 0; }

#endif /*BENCHMARK*/

#ifndef M_PI
#define M_PI 3.14159265
#endif

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLint   gear1, gear2, gear3;
static GLfloat angle = 0.0;

/*
 *
 *  Draw a gear wheel.  You'll probably want to call this function when
 *  building a display list since we do a lot of trig here.
 *
 *  Input:  inner_radius - radius of hole at center
 *          outer_radius - radius at center of teeth
 *          width - width of gear
 *          teeth - number of teeth
 *          tooth_depth - depth of tooth
 */
static void gear(GLfloat inner_radius,
                 GLfloat outer_radius,
                 GLfloat width,
                 GLint   teeth,
                 GLfloat tooth_depth)
{
  GLint   i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * M_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);

  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    if (i < teeth) {
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
                 width * 0.5);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    if (i < teeth) {
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
                 -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
               -width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
               -width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    u   = r2 * cos(angle + da) - r1 * cos(angle);
    v   = r2 * sin(angle + da) - r1 * sin(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
               -width * 0.5);
    u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
    v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
               -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
  }
  glEnd();
}

static void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(view_rotx, 1.0, 0.0, 0.0);
  glRotatef(view_roty, 0.0, 1.0, 0.0);
  glRotatef(view_rotz, 0.0, 0.0, 1.0);

  glPushMatrix();
  glTranslatef(-3.0, -2.0, 0.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  glCallList(gear1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(3.1, -2.0, 0.0);
  glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
  glCallList(gear2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-3.1, 4.2, 0.0);
  glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
  glCallList(gear3);
  glPopMatrix();

  glPopMatrix();
}

/* new window size or exposure */
static void reshape(int width, int height)
{
  GLfloat h = (GLfloat)height / (GLfloat)width;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);
}

static void init(void)
{
  static GLfloat pos[4]   = {5.0, 5.0, 10.0, 0.0};
  static GLfloat red[4]   = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4]  = {0.2, 0.2, 1.0, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  /* make the gears */
  gear1 = glGenLists(1);
  glNewList(gear1, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  gear(1.0, 4.0, 1.0, 20, 0.7);
  glEndList();

  gear2 = glGenLists(1);
  glNewList(gear2, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  gear(0.5, 2.0, 2.0, 10, 0.7);
  glEndList();

  gear3 = glGenLists(1);
  glNewList(gear3, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
  gear(1.3, 2.0, 0.5, 10, 0.7);
  glEndList();

  glEnable(GL_NORMALIZE);
}

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

typedef enum {                                                             
  GHOST_kTabletModeNone = 0,                                               
  GHOST_kTabletModeStylus,                                                 
  GHOST_kTabletModeEraser                                                  
} GHOST_TTabletMode;

/* These C functions are copied from Wine 3.12's wintab.c */               
static bool match_token(const char *haystack, const char *needle)          
{                                                                          
  const char *p, *q;                                                       
  for (p = haystack; *p;) {                                                
    while (*p && isspace(*p))                                              
      p++;                                                                 
    if (!*p)                                                               
      break;                                                               
                                                                           
    for (q = needle; *q && *p && tolower(*p) == tolower(*q); q++)          
      p++;                                                                 
    if (!*q && (isspace(*p) || !*p))                                       
      return true;                                                         
                                                                           
    while (*p && !isspace(*p))                                             
      p++;                                                                 
  }                                                                        
  return false;                                                            
}

/* Determining if an X device is a Tablet style device is an imperfect science.
 * We rely on common conventions around device names as well as the type reported
 * by Wacom tablets.  This code will likely need to be expanded for alternate tablet types
 *                                                                         
 * Wintab refers to any device that interacts with the tablet as a cursor, 
 * (stylus, eraser, tablet mouse, airbrush, etc)                           
 * this is not to be confused with wacom x11 configuration "cursor" device.
 * Wacoms x11 config "cursor" refers to its device slot (which we mirror with
 * our gSysCursors) for puck like devices (tablet mice essentially).       
 */                                                                        
static GHOST_TTabletMode tablet_mode_from_name(const char *name, const char *type)
{                                                                          
  int i;                                                                   
  static const char *tablet_stylus_whitelist[] = {"stylus", "wizardpen", "acecad", "pen", NULL};
                                                                           
  static const char *type_blacklist[] = {"pad", "cursor", "touch", NULL};  
                                                                           
  /* Skip some known unsupported types. */                                 
  for (i = 0; type_blacklist[i] != NULL; i++) {                            
    if (type && (strcasecmp(type, type_blacklist[i]) == 0)) {              
      return GHOST_kTabletModeNone;                                        
    }                                                                      
  }                                                                        
                                                                           
  /* First check device type to avoid cases where name is "Pen and Eraser" and type is "ERASER" */
  for (i = 0; tablet_stylus_whitelist[i] != NULL; i++) {                   
    if (type && match_token(type, tablet_stylus_whitelist[i])) {           
      return GHOST_kTabletModeStylus;                                      
    }                                                                      
  }                                                                        
  if (type && match_token(type, "eraser")) {                               
    return GHOST_kTabletModeEraser;                                        
  }                                                                        
  for (i = 0; tablet_stylus_whitelist[i] != NULL; i++) {                   
    if (name && match_token(name, tablet_stylus_whitelist[i])) {           
      return GHOST_kTabletModeStylus;                                      
    }                                                                      
  }                                                                        
  if (name && match_token(name, "eraser")) {                               
    return GHOST_kTabletModeEraser;                                        
  }                                                                        
                                                                           
  return GHOST_kTabletModeNone;                                            
}




class Dev{
  public:
    Dev(XDevice*const&d,XID i,int presL,int xtilt,int ytilt):device(d),id(i),pressureLevels(presL),xTiltLevels(xtilt),yTiltLevels(ytilt){}
    XDevice*device;
    XID id;
    int pressureLevels;
    int xTiltLevels;
    int yTiltLevels;
    int MotionEvent;                                                       
    int ProxInEvent;                                                       
    int ProxOutEvent;                                                      
    int PressEvent;
};

// copied and modified from blender intern/ghost/intern/GHOST_SystemX11.cpp from blender
std::vector<Dev>getDevices(Display*dpy){
  std::vector<Dev>result;
  int ndev;
  ___;
  auto devInfo = XListInputDevices(dpy,&ndev);
  ___;
  for(int i=0;i<ndev;++i){
    char *device_type = devInfo[i].type ? XGetAtomName(dpy, devInfo[i].type) :
                                              NULL;                    
    GHOST_TTabletMode tablet_mode = tablet_mode_from_name(devInfo[i].name, device_type);
                                                                       
    // printf("Tablet type:'%s', name:'%s', index:%d\n", device_type, device_info[i].name, i);
                                                                       
    if (device_type) {                                                 
      XFree((void *)device_type);                                      
    }                                                                  
                                                                       
    if (!(tablet_mode == GHOST_kTabletModeStylus || tablet_mode == GHOST_kTabletModeEraser)) {
      continue;                                                        
    }   



    ___;
    auto openDev = XOpenDevice(dpy,devInfo[i].id);
    ___;
    if(!openDev)continue;

                                                                     
      /* Find how many pressure levels tablet has */                   
    XAnyClassPtr ici = devInfo[i].inputclassinfo;                
                                          
    int PressureLevels = 0; 
    int XtiltLevels = 0;
    int YtiltLevels = 0;
    for (int j = 0; j < openDev->num_classes; ++j) {          
      if (ici->c_class == ValuatorClass) {
        XValuatorInfo *xvi = (XValuatorInfo *)ici;                   
        auto PressureLevels = xvi->axes[2].max_value;             
                                                                     
        if (xvi->num_axes > 3) {                                     
          /* this is assuming that the tablet has the same tilt resolution in both
           * positive and negative directions. It would be rather weird if it didn't.. */
          XtiltLevels = xvi->axes[3].max_value;              
          YtiltLevels = xvi->axes[4].max_value;              
        }                                                            
        else {                                                       
          XtiltLevels = 0;                                   
          YtiltLevels = 0;                                   
        }                                                            
        break;                                                       
      }                                                              
      ici = (XAnyClassPtr)(((char *)ici) + ici->length);             
    }
    result.emplace_back(openDev,devInfo[i].id,PressureLevels,XtiltLevels,YtiltLevels);

  }
  ___;
  XFreeDeviceList(devInfo);
  ___;
  return result;
}

void freeDevices(std::vector<Dev>&devs,Display*dpy){
  for(auto const&d:devs)
    if(d.device)XCloseDevice(dpy,d.device);
  devs.clear();
}


/*
 * Create an RGB, double-buffered window.
 * Return the window and context handles.
 */
static void make_window(Display *   dpy,
                        const char *name,
                        int         x,
                        int         y,
                        int         width,
                        int         height,
                        Window *    winRet,
                        GLXContext *ctxRet)
{
  int                  attrib[] = {GLX_RGBA,
                  GLX_RED_SIZE,
                  1,
                  GLX_GREEN_SIZE,
                  1,
                  GLX_BLUE_SIZE,
                  1,
                  GLX_DOUBLEBUFFER,
                  GLX_DEPTH_SIZE,
                  1,
                  None};
  int                  scrnum;
  XSetWindowAttributes attr;
  unsigned long        mask;
  Window               root;
  Window               win;
  GLXContext           ctx;
  XVisualInfo *        visinfo;

  scrnum = DefaultScreen(dpy);
  root   = RootWindow(dpy, scrnum);

  visinfo = glXChooseVisual(dpy, scrnum, attrib);
  if (!visinfo) {
    printf("Error: couldn't get an RGB, Double-buffered visual\n");
    exit(1);
  }

  /* window attributes */
  attr.background_pixel = 0;
  attr.border_pixel     = 0;
  attr.colormap   = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
  attr.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                          EnterWindowMask | LeaveWindowMask | ButtonPressMask |
                          ButtonReleaseMask | PointerMotionMask | FocusChangeMask |
                          PropertyChangeMask | KeymapStateMask; 

  Atom tablet = XInternAtom(dpy, XI_TABLET, False);

  //attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | PointerMotionMask;
  mask            = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

  win = XCreateWindow(dpy, root, 0, 0, width, height, 0, visinfo->depth,
                      InputOutput, visinfo->visual, mask, &attr);

  /* set hints and properties */
  {
    XSizeHints sizehints;
    sizehints.x      = x;
    sizehints.y      = y;
    sizehints.width  = width;
    sizehints.height = height;
    sizehints.flags  = USSize | USPosition;
    XSetNormalHints(dpy, win, &sizehints);
    XSetStandardProperties(dpy, win, name, name, None, (char **)NULL, 0,
                           &sizehints);
  }


  auto devs = getDevices(dpy);
  std::vector<XEventClass> xevents;                                      
                                                                         
  for (auto &xtablet : devs){
    /* With modern XInput (xlib 1.6.2 at least and/or evdev 2.9.0) and some 'no-name' tablets
     * like 'UC-LOGIC Tablet WP5540U', we also need to 'select' ButtonPress for motion event,
     * otherwise we do not get any tablet motion event once pen is pressed... See T43367.
     */                                                                  
    XEventClass ev;                                                      
                                                                         
    DeviceMotionNotify(xtablet.device, xtablet.MotionEvent, ev);         
    if (ev)                                                              
      xevents.push_back(ev);                                             
    DeviceButtonPress(xtablet.device, xtablet.PressEvent, ev);           
    if (ev)                                                              
      xevents.push_back(ev);                                             
    ProximityIn(xtablet.device, xtablet.ProxInEvent, ev);                
    if (ev)                                                              
      xevents.push_back(ev);                                             
    ProximityOut(xtablet.device, xtablet.ProxOutEvent, ev);              
    if (ev)                                                              
      xevents.push_back(ev);                                             
  }                                                                      
                                                                         
  XSelectExtensionEvent(dpy, win, xevents.data(), (int)xevents.size());





  // ctx = glXCreateContext( dpy, visinfo, NULL, True );
  // glXCreateNewContext(dpy,visinfo,)
  // glXDestroyContext(dpy,ctx);
  ctx = glXCreateContext(dpy, visinfo, NULL, True);
  if (!ctx) {
    printf("Error: glXCreateContext failed\n");
    exit(1);
  }

  XFree(visinfo);

  *winRet = win;
  *ctxRet = ctx;
}

static void event_loop(Display *dpy, Window win)
{
  while (1) {
    while (XPending(dpy) > 0) {
      XEvent event;
      XNextEvent(dpy, &event);
      fprintf(stderr, "type: %u\n", event.type);
      auto xe = &event;
      XDeviceMotionEvent *dd = (XDeviceMotionEvent *)xe;
      XDeviceMotionEvent *data = (XDeviceMotionEvent *)xe;
      switch (event.type) {
        case 71:
          fprintf(stderr,"f: %u\n",data->axis_data[2]);
          break;
        case Expose:
          /* we'll redraw below */
          break;
        case ConfigureNotify:
          reshape(event.xconfigure.width, event.xconfigure.height);
          break;
        case KeyPress: {
          char buffer[10];
          int  r, code;
          code = XLookupKeysym(&event.xkey, 0);
          if (code == XK_Left) {
            view_roty += 5.0;
          }
          else if (code == XK_Right) {
            view_roty -= 5.0;
          }
          else if (code == XK_Up) {
            view_rotx += 5.0;
          }
          else if (code == XK_Down) {
            view_rotx -= 5.0;
          }
          else {
            r = XLookupString(&event.xkey, buffer, sizeof(buffer), NULL, NULL);
            if (buffer[0] == 27) {
              /* escape */
              return;
            }
          }
        }
      }
    }

    /* next frame */
    angle += 2.0;

    draw();
    glXSwapBuffers(dpy, win);

    /* calc framerate */
    {
      static int t0     = -1;
      static int frames = 0;
      int        t      = current_time();

      if (t0 < 0) t0 = t;

      frames++;

      if (t - t0 >= 5.0) {
        GLfloat seconds = t - t0;
        GLfloat fps     = frames / seconds;
        printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds,
               fps);
        t0     = t;
        frames = 0;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  Display *  dpy;
  Window     win;
  GLXContext ctx;
  char *     dpyName   = NULL;
  GLboolean  printInfo = GL_FALSE;
  int        i;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-display") == 0) {
      dpyName = argv[i + 1];
      i++;
    }
    else if (strcmp(argv[i], "-info") == 0) {
      printInfo = GL_TRUE;
    }
  }

  dpy = XOpenDisplay(dpyName);
  if (!dpy) {
    printf("Error: couldn't open display %s\n", dpyName);
    return -1;
  }

  make_window(dpy, "glxgears", 0, 0, 300, 300, &win, &ctx);
  XMapWindow(dpy, win);
  glXMakeCurrent(dpy, win, ctx);
  reshape(300, 300);

  if (printInfo) {
    printf("GL_RENDERER   = %s\n", (char *)glGetString(GL_RENDERER));
    printf("GL_VERSION    = %s\n", (char *)glGetString(GL_VERSION));
    printf("GL_VENDOR     = %s\n", (char *)glGetString(GL_VENDOR));
    printf("GL_EXTENSIONS = %s\n", (char *)glGetString(GL_EXTENSIONS));
  }

  init();

  event_loop(dpy, win);

  glXDestroyContext(dpy, ctx);
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);

  return 0;
}
