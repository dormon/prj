#include <imguiSDL2Dormon/EventHandlerImpl.h>

// SDL
// (the multi-viewports feature requires SDL features supported from SDL 2.0.5+)
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#define SDL_HAS_WARP_MOUSE_GLOBAL           SDL_VERSION_ATLEAST(2,0,4)

#define SDL_HAS_CAPTURE_MOUSE               SDL_VERSION_ATLEAST(2,0,4)
#define SDL_HAS_VULKAN                      SDL_VERSION_ATLEAST(2,0,6)
#define SDL_HAS_MOUSE_FOCUS_CLICKTHROUGH    SDL_VERSION_ATLEAST(2,0,5)

namespace imguiSDL2Dormon{

// Data
//static char*        clipboardTextData = NULL;

const char* getClipboardText(void*data){
  auto clipboardTextData = reinterpret_cast<char**>(data);
  if (*clipboardTextData)
    SDL_free(*clipboardTextData);
  *clipboardTextData = SDL_GetClipboardText();
  return *clipboardTextData;
}

void setClipboardText(void*, const char* text){
  SDL_SetClipboardText(text);
}

void EventHandlerImpl::setMousePosition(){
  SDL_Window* focused_window = SDL_GetKeyboardFocus();
  
  if (window != focused_window)return;
  // SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
  // The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
  int wx, wy;
  SDL_GetWindowPosition(focused_window, &wx, &wy);
 
  int mx, my;
  SDL_GetGlobalMouseState(&mx, &my);
  
  mx -= wx;
  my -= wy;
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2((float)mx, (float)my);
}

void EventHandlerImpl::captureMouse(){
  // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor. 
  // The function is only supported from SDL 2.0.4 (released Jan 2016)
  bool any_mouse_button_down = ImGui::IsAnyMouseDown();
  SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
}

void EventHandlerImpl::updateMousePosAndButtons()
{
  ImGuiIO& io = ImGui::GetIO();
  
  const ImVec2 mouse_pos_backup = io.MousePos;
  io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
  

  // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
  // (When multi-viewports are enabled, all imgui positions are same as OS positions.)
#if SDL_HAS_WARP_MOUSE_GLOBAL
  if (io.WantSetMousePos)
    SDL_WarpMouseGlobal((int)mouse_pos_backup.x, (int)mouse_pos_backup.y);
#endif
  

  int mx, my;
  Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
  io.MouseDown[0] = mousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT  )) != 0;
  io.MouseDown[1] = mousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT )) != 0;
  io.MouseDown[2] = mousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  mousePressed[0] = mousePressed[1] = mousePressed[2] = false;
  

#if SDL_HAS_CAPTURE_MOUSE
  setMousePosition();
  captureMouse();
#else
  if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
    io.MousePos = ImVec2((float)mx, (float)my);
#endif
  
}

void EventHandlerImpl::hideCursor(){
  // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
  SDL_ShowCursor(SDL_FALSE);
}

void EventHandlerImpl::showAndSetCursor(){
  ImGuiIO& io = ImGui::GetIO();
  ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
  SDL_SetCursor(mouseCursors[imgui_cursor] ? mouseCursors[imgui_cursor] : mouseCursors[ImGuiMouseCursor_Arrow]);
  SDL_ShowCursor(SDL_TRUE);
}

bool EventHandlerImpl::wasCursorNotChanged(){
  ImGuiIO& io = ImGui::GetIO();
  return io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange;
}

bool EventHandlerImpl::isCursorNotVisible(){
  ImGuiIO& io = ImGui::GetIO();
  ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
  return io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None;
}

void EventHandlerImpl::updateMouseCursor(){
  if (wasCursorNotChanged())return;
  if (isCursorNotVisible())hideCursor();
  else showAndSetCursor();
}

void EventHandlerImpl::fillKeyMap(){
  ImGuiIO& io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Tab       ] = SDL_SCANCODE_TAB      ;
  io.KeyMap[ImGuiKey_LeftArrow ] = SDL_SCANCODE_LEFT     ;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT    ;
  io.KeyMap[ImGuiKey_UpArrow   ] = SDL_SCANCODE_UP       ;
  io.KeyMap[ImGuiKey_DownArrow ] = SDL_SCANCODE_DOWN     ;
  io.KeyMap[ImGuiKey_PageUp    ] = SDL_SCANCODE_PAGEUP   ;
  io.KeyMap[ImGuiKey_PageDown  ] = SDL_SCANCODE_PAGEDOWN ;
  io.KeyMap[ImGuiKey_Home      ] = SDL_SCANCODE_HOME     ;
  io.KeyMap[ImGuiKey_End       ] = SDL_SCANCODE_END      ;
  io.KeyMap[ImGuiKey_Insert    ] = SDL_SCANCODE_INSERT   ;
  io.KeyMap[ImGuiKey_Delete    ] = SDL_SCANCODE_DELETE   ;
  io.KeyMap[ImGuiKey_Backspace ] = SDL_SCANCODE_BACKSPACE;
  io.KeyMap[ImGuiKey_Space     ] = SDL_SCANCODE_SPACE    ;
  io.KeyMap[ImGuiKey_Enter     ] = SDL_SCANCODE_RETURN   ;
  io.KeyMap[ImGuiKey_Escape    ] = SDL_SCANCODE_ESCAPE   ;
  io.KeyMap[ImGuiKey_A         ] = SDL_SCANCODE_A        ;
  io.KeyMap[ImGuiKey_C         ] = SDL_SCANCODE_C        ;
  io.KeyMap[ImGuiKey_V         ] = SDL_SCANCODE_V        ;
  io.KeyMap[ImGuiKey_X         ] = SDL_SCANCODE_X        ;
  io.KeyMap[ImGuiKey_Y         ] = SDL_SCANCODE_Y        ;
  io.KeyMap[ImGuiKey_Z         ] = SDL_SCANCODE_Z        ;
}

void EventHandlerImpl::fillMouseCursors(){
  mouseCursors[ImGuiMouseCursor_Arrow     ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW   );
  mouseCursors[ImGuiMouseCursor_TextInput ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM   );
  mouseCursors[ImGuiMouseCursor_ResizeAll ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL );
  mouseCursors[ImGuiMouseCursor_ResizeNS  ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS  );
  mouseCursors[ImGuiMouseCursor_ResizeEW  ] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE  );
  mouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  mouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
}

EventHandlerImpl::EventHandlerImpl(SDL_Window*win,void*){
  window = win;

  // Setup back-end capabilities flags
  ImGuiIO& io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
#if SDL_HAS_WARP_MOUSE_GLOBAL
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
#endif

  // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
  fillKeyMap();

  io.SetClipboardTextFn = setClipboardText;
  io.GetClipboardTextFn = getClipboardText;
  io.ClipboardUserData = &clipboardTextData;

  fillMouseCursors();

#ifdef _WIN32
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(window, &wmInfo);
  io.ImeWindowHandle = wmInfo.info.win.window;
#endif

}

EventHandlerImpl::~EventHandlerImpl(){
  window = NULL;

  // Destroy last known clipboard data
  if (clipboardTextData)
    SDL_free(clipboardTextData);
  clipboardTextData = NULL;

  // Destroy SDL mouse cursors
  for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
    SDL_FreeCursor(mouseCursors[cursor_n]);
  memset(mouseCursors, 0, sizeof(mouseCursors));
}

void EventHandlerImpl::newFrame(SDL_Window*window){
  ImGuiIO& io = ImGui::GetIO();
  IM_ASSERT(io.Fonts->IsBuilt());     // Font atlas needs to be built, call renderer _NewFrame() function e.g. ImGui_ImplOpenGL3_NewFrame() 
  

  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  
  int display_w, display_h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_GL_GetDrawableSize(window, &display_w, &display_h);
  
  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);
  

  // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
  static Uint64 frequency = SDL_GetPerformanceFrequency();
  Uint64 current_time = SDL_GetPerformanceCounter();
  io.DeltaTime = time > 0 ? (float)((double)(current_time - time) / frequency) : (float)(1.0f / 60.0f);
  time = current_time;
  

  updateMousePosAndButtons();
  
  updateMouseCursor();
}

bool EventHandlerImpl::processMouseWheel(SDL_Event*event){
  ImGuiIO& io = ImGui::GetIO();
  if (event->wheel.x > 0) io.MouseWheelH += 1;
  if (event->wheel.x < 0) io.MouseWheelH -= 1;
  if (event->wheel.y > 0) io.MouseWheel  += 1;
  if (event->wheel.y < 0) io.MouseWheel  -= 1;
  return true;
}

bool EventHandlerImpl::processMouseButtonDown(SDL_Event*event){
  if (event->button.button == SDL_BUTTON_LEFT  ) mousePressed[0] = true;
  if (event->button.button == SDL_BUTTON_RIGHT ) mousePressed[1] = true;
  if (event->button.button == SDL_BUTTON_MIDDLE) mousePressed[2] = true;
  return true;
}

bool EventHandlerImpl::processTextInput(SDL_Event*event){
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharactersUTF8(event->text.text);
  return true;
}

bool EventHandlerImpl::processKey(SDL_Event*event){
  ImGuiIO& io = ImGui::GetIO();
  int key = event->key.keysym.scancode;
  IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
  io.KeysDown[key] = (event->type == SDL_KEYDOWN);
  io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
  io.KeyCtrl  = ((SDL_GetModState() & KMOD_CTRL ) != 0);
  io.KeyAlt   = ((SDL_GetModState() & KMOD_ALT  ) != 0);
  io.KeySuper = ((SDL_GetModState() & KMOD_GUI  ) != 0);
  return true;
}

bool EventHandlerImpl::processEvent(SDL_Event* event){
  switch (event->type){
    case SDL_MOUSEWHEEL     :return processMouseWheel     (event);
    case SDL_MOUSEBUTTONDOWN:return processMouseButtonDown(event);
    case SDL_TEXTINPUT      :return processTextInput      (event);
    case SDL_KEYDOWN        :
    case SDL_KEYUP          :return processKey            (event);
  }
  return false;
}

}
