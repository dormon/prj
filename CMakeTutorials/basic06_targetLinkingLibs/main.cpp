#include<iostream>

#include<SDL2/SDL.h>

int main(int,char*[]){
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(
      "test",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,                    
      480,                    
      SDL_WINDOW_OPENGL       
      );

  SDL_Delay(3000);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
