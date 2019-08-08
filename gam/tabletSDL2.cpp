#include <SDL_syswm.h>
#include <X11/extensions/XInput.h>
#include <tabletSDL2.h>
#include <vector>

/* These C functions are copied from Wine 3.12's wintab.c */
static bool match_token(const char *haystack, const char *needle)
{
  const char *p, *q;
  for (p = haystack; *p;) {
    while (*p && isspace(*p)) p++;
    if (!*p) break;

    for (q = needle; *q && *p && tolower(*p) == tolower(*q); q++) p++;
    if (!*q && (isspace(*p) || !*p)) return true;

    while (*p && !isspace(*p)) p++;
  }
  return false;
}

/* Determining if an X device is a Tablet style device is an imperfect science.
 * We rely on common conventions around device names as well as the type
 * reported by Wacom tablets.  This code will likely need to be expanded for
 * alternate tablet types
 *
 * Wintab refers to any device that interacts with the tablet as a cursor,
 * (stylus, eraser, tablet mouse, airbrush, etc)
 * this is not to be confused with wacom x11 configuration "cursor" device.
 * Wacoms x11 config "cursor" refers to its device slot (which we mirror with
 * our gSysCursors) for puck like devices (tablet mice essentially).
 */
static TabletData::Mode tablet_mode_from_name(const char *name,
                                              const char *type)
{
  int                i;
  static const char *tablet_stylus_whitelist[] = {"stylus", "wizardpen",
                                                  "acecad", "pen", NULL};

  static const char *type_blacklist[] = {"pad", "cursor", "touch", NULL};

  /* Skip some known unsupported types. */
  for (i = 0; type_blacklist[i] != NULL; i++) {
    if (type && (strcasecmp(type, type_blacklist[i]) == 0)) {
      return TabletData::Mode::NoneMode;
    }
  }

  /* First check device type to avoid cases where name is "Pen and Eraser" and
   * type is "ERASER" */
  for (i = 0; tablet_stylus_whitelist[i] != NULL; i++) {
    if (type && match_token(type, tablet_stylus_whitelist[i])) {
      return TabletData::Mode::Stylus;
    }
  }
  if (type && match_token(type, "eraser")) {
    return TabletData::Mode::Eraser;
  }
  for (i = 0; tablet_stylus_whitelist[i] != NULL; i++) {
    if (name && match_token(name, tablet_stylus_whitelist[i])) {
      return TabletData::Mode::Stylus;
    }
  }
  if (name && match_token(name, "eraser")) {
    return TabletData::Mode::Eraser;
  }

  return TabletData::Mode::NoneMode;
}

class Dev {
 public:
  Dev(XDevice *const &d, XID i, int presL, int xtilt, int ytilt)
      : device(d),
        id(i),
        pressureLevels(presL),
        xTiltLevels(xtilt),
        yTiltLevels(ytilt)
  {
  }
  TabletData::Mode mode;
  XDevice *        device;
  XID              id;
  int              pressureLevels;
  int              xTiltLevels;
  int              yTiltLevels;
  int              MotionEvent;
  int              ProxInEvent;
  int              ProxOutEvent;
  int              PressEvent;
};

// copied and modified from blender intern/ghost/intern/GHOST_SystemX11.cpp from
// blender
std::vector<Dev> getDevices(Display *dpy)
{
  std::vector<Dev> result;
  int              ndev;
  auto             devInfo = XListInputDevices(dpy, &ndev);
  for (int i = 0; i < ndev; ++i) {
    char *device_type =
        devInfo[i].type ? XGetAtomName(dpy, devInfo[i].type) : NULL;
    TabletData::Mode tablet_mode =
        tablet_mode_from_name(devInfo[i].name, device_type);

    // printf("Tablet type:'%s', name:'%s', index:%d\n", device_type,
    // device_info[i].name, i);

    if (device_type) {
      XFree((void *)device_type);
    }

    if (!(tablet_mode == TabletData::Mode::Stylus ||
          tablet_mode == TabletData::Mode::Eraser)) {
      continue;
    }

    auto openDev = XOpenDevice(dpy, devInfo[i].id);
    if (!openDev) continue;

    /* Find how many pressure levels tablet has */
    XAnyClassPtr ici = devInfo[i].inputclassinfo;

    int PressureLevels = 0;
    int XtiltLevels    = 0;
    int YtiltLevels    = 0;
    for (int j = 0; j < openDev->num_classes; ++j) {
      if (ici->c_class == ValuatorClass) {
        XValuatorInfo *xvi = (XValuatorInfo *)ici;
        PressureLevels     = xvi->axes[2].max_value;

        if (xvi->num_axes > 3) {
          /* this is assuming that the tablet has the same tilt resolution in
           * both
           * positive and negative directions. It would be rather weird if it
           * didn't.. */
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
    result.emplace_back(openDev, devInfo[i].id, PressureLevels, XtiltLevels,
                        YtiltLevels);
  }
  XFreeDeviceList(devInfo);
  return result;
}

void freeDevices(std::vector<Dev> &devs, Display *dpy)
{
  for (auto const &d : devs)
    if (d.device) XCloseDevice(dpy, d.device);
  devs.clear();
}

class TabletSDL2Impl {
 public:
  TabletSDL2Impl(SDL_Window *sdlWin)
  {
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(sdlWin, &info);
    dpy  = info.info.x11.display;
    ww   = info.info.x11.window;
    devs = getDevices(dpy);

    std::vector<XEventClass> xevents;

    for (auto &xtablet : devs) {
      /* With modern XInput (xlib 1.6.2 at least and/or evdev 2.9.0) and some
       * 'no-name' tablets like 'UC-LOGIC Tablet WP5540U', we also need to
       * 'select' ButtonPress for motion event, otherwise we do not get any
       * tablet motion event once pen is pressed... See T43367.
       */
      XEventClass ev;

      DeviceMotionNotify(xtablet.device, xtablet.MotionEvent, ev);
      if (ev) xevents.push_back(ev);
      DeviceButtonPress(xtablet.device, xtablet.PressEvent, ev);
      if (ev) xevents.push_back(ev);
      ProximityIn(xtablet.device, xtablet.ProxInEvent, ev);
      if (ev) xevents.push_back(ev);
      ProximityOut(xtablet.device, xtablet.ProxOutEvent, ev);
      if (ev) xevents.push_back(ev);
    }

    XSelectExtensionEvent(dpy, ww, xevents.data(), (int)xevents.size());
  }
  ~TabletSDL2Impl() { freeDevices(devs, dpy); }
  bool getTabletData(TabletData *ddd)
  {
    if (XPending(dpy)) {
      XEvent ee;
      XPeekEvent(dpy, &ee);
      auto xe = &ee;
      for (auto &xtablet : devs) {
        XDeviceMotionEvent *dd = (XDeviceMotionEvent *)xe;
        if (xe->type == xtablet.MotionEvent || xe->type == xtablet.PressEvent) {
          XDeviceMotionEvent *data = (XDeviceMotionEvent *)xe;
          if (data->deviceid != xtablet.id) {
            continue;
          }

          const unsigned char axis_first = data->first_axis;
          const unsigned char axes_end =
              axis_first + data->axes_count; /* after the last */
          int axis_value;

          /* stroke might begin without leading ProxyIn event,
           * this happens when window is opened when stylus is already hovering
           * around tablet surface */
          ddd->active = xtablet.mode;

          /* Note: This event might be generated with incomplete dataset
           * (don't exactly know why, looks like in some cases, if the value
           * does not change, it is not included in subsequent
           * XDeviceMotionEvent events). So we have to check which values this
           * event actually contains!
           */

#define AXIS_VALUE_GET(axis, val)             \
  ((axis_first <= axis && axes_end > axis) && \
   ((void)(val = data->axis_data[axis - axis_first]), true))

          if (AXIS_VALUE_GET(2, axis_value)) {
            ddd->pressure = axis_value / ((float)xtablet.pressureLevels);
          }

          /* the (short) cast and the & 0xffff is bizarre and unexplained
           * anywhere, but I got garbage data without it. Found it in the
           * xidump.c source --matt
           *
           * The '& 0xffff' just truncates the value to its two lowest bytes,
           * this probably means some drivers do not properly set the whole int
           * value? Since we convert to float afterward, I don't think we need
           * to cast to short here, but do not have a device to check this.
           * --mont29
           */
          if (AXIS_VALUE_GET(3, axis_value)) {
            ddd->xTilt =
                (short)(axis_value & 0xffff) / ((float)xtablet.xTiltLevels);
          }
          if (AXIS_VALUE_GET(4, axis_value)) {
            ddd->yTilt =
                (short)(axis_value & 0xffff) / ((float)xtablet.yTiltLevels);
          }
          return true;

#undef AXIS_VALUE_GET
        }
        else if (xe->type == xtablet.ProxInEvent) {
          XProximityNotifyEvent *data = (XProximityNotifyEvent *)xe;
          if (data->deviceid != xtablet.id) {
            continue;
          }
          return true;
        }
        else if (xe->type == xtablet.ProxOutEvent) {
          ddd->active = TabletData::Mode::NoneMode;
          return true;
        }
      }
    }
    return false;
  }
  Display *        dpy;
  Window           ww;
  std::vector<Dev> devs;
};

TabletSDL2::TabletSDL2(SDL_Window *window)
{
  impl = new TabletSDL2Impl(window);
}

TabletSDL2::~TabletSDL2() { delete impl; }

bool TabletSDL2::getTabletData(TabletData *data)
{
  return impl->getTabletData(data);
}
