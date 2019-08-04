#include <Vars/Vars.h>
#include <SDL.h>

void destroyWindow(vars::Vars&vars){
  auto window = *vars.get<SDL_Window*>("window");
  auto context = *vars.get<SDL_GLContext>("context");
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
}

