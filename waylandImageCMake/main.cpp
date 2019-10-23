#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <Vars/Vars.h>
#include "Timer.h"

#include <functional>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define DEBUG

#include "util.h"
#include "Window.h"
#include "QuiltImage.h"

using namespace ge::gl;

class SimpleWindow: public Window{
  public:
    SimpleWindow(uint32_t w,uint32_t h,bool fullscreen,int argc,char*argv[]):Window(w,h,fullscreen),argc(argc),argv(argv){
      std::cerr << "SimpleWindow::SimpleWindow()" << std::endl;
    }
    virtual ~SimpleWindow(){
      std::cerr << "SimpleWindow::~SimpleWindow()" << std::endl;
    }
    virtual void onInit()override;
    virtual void onDraw()override;
  protected:
    int argc;
    char**argv;
    vars::Vars vars;
    bool running = true;
    std::shared_ptr<ge::gl::Program>prg;
    std::shared_ptr<ge::gl::Framebuffer>fbo;
    std::shared_ptr<QuiltImage>quiltImage;
    GLuint m_texture;
};


void SimpleWindow::onInit(){
  ___;
  ge::gl::init((ge::gl::GET_PROC_ADDRESS)eglGetProcAddress);
  ___;
  std::cerr << ge::gl::glGetString(GL_VERSION) << std::endl;
  //quiltImage = std::make_shared<QuiltImage>(argv[1],vars);
}

void SimpleWindow::onDraw(){
  Timer<float>timer;
  timer.reset();

  //ge::gl::glClearColor(0,0.1,0,1);
  //ge::gl::glClear(GL_COLOR_BUFFER_BIT);

  //quiltImage->draw();
  ge::gl::glFinish();

  eglSurface.swap();

  auto t = timer.elapsedFromStart();
  std::cerr << "frame time: " << t << " - fps: " << 1/t << std::endl;
}



int main(int argc,char*argv[])
{
  auto win = SimpleWindow(1920*2,1080*2,true,argc,argv);
  //auto win = SimpleWindow(800,600,false);
  win.start();

  return EXIT_SUCCESS;
}
