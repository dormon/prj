#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <Vars/Vars.h>

#include <SDL.h>
#include <GL/glew.h>
#include <createWindow.h>
#include <destroyWindow.h>
#include <tabletSDL2.h>
#include <SDL_syswm.h>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl


int main(){
  vars::Vars vars;

  createWindow(vars);

  auto&running = vars.addBool("mainLoopRunning",true);


  auto window = *vars.get<SDL_Window*>("window");

  auto tabletSDL2 = TabletSDL2(window);


  SDL_Event event;
  while(running){

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          std::cerr << "keydown" << std::endl;
          break;
        case SDL_KEYUP:
          break;
        case SDL_MOUSEMOTION:
          //std::cerr << "x: " << event.motion.x << " y: " << event.motion.y << std::endl;
          break;
        case SDL_MOUSEBUTTONDOWN:
          break;
        case SDL_MOUSEBUTTONUP:
          break;
        case SDL_FINGERUP:
          std::cerr << "finger up" << std::endl;
          break;
        case SDL_FINGERDOWN:
          std::cerr << "finger down" << std::endl;
          break;
        case SDL_FINGERMOTION:
          std::cerr << "finger motion" << std::endl;
          break;
        case SDL_SYSWMEVENT:
          {
            TabletData td;
            if(tabletSDL2.getTabletData(&td,event.syswm.msg->msg.x11.event)){
              std::cerr << td.pressure << std::endl;
            }
          }
          break;
      }
    }

    glClearColor(0,0.05,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(*vars.get<SDL_Window*>("window"));
  //std::cerr << "GetNumTouchFingers:" << SDL_GetNumTouchFingers(0) << std::endl;
  //std::cerr << "GetNumTouch: " << SDL_GetNumTouchDevices() << std::endl;

  }

  destroyWindow(vars);

  return 0;
}


