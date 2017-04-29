#include<SDL2/SDL.h>
#include"GetProcAddressInterface.h"

extern "C"{
  class SDL2GetProcAddress: public GetProcAddress{
    public:
      void*get(const char*name){
        return SDL_GL_GetProcAddress(name);
      }
  };

SDL2GetProcAddress*getObject(){
  return new SDL2GetProcAddress();
}
  
}
