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


#include <dirent.h>
#include <linux/input.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define DEBUG

#include "util.h"
#include "Window.h"
#include "QuiltImage.h"

using namespace ge::gl;

std::vector<std::string>getDirFiles(std::string const&directory){
  DIR *dir;
  struct dirent *ent;
  std::vector<std::string>res;
  if ((dir = opendir (directory.c_str())) != NULL) {
    int Counter=-1;
    while ((ent = readdir (dir)) != NULL) {
      Counter++;
      if(ent->d_type==DT_DIR)continue;
      res.push_back(std::string(directory)+std::string(ent->d_name));
    }
    closedir (dir);
  } else {
    perror ("");
  }
  return res;
}


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
    virtual void onKeyDown(uint32_t)override;
  protected:
    int argc;
    char**argv;
    vars::Vars vars;
    bool running = true;
    std::shared_ptr<ge::gl::Program>prg;
    std::shared_ptr<ge::gl::Framebuffer>fbo;
    std::shared_ptr<QuiltImage>quiltImage;
    size_t selectedQuilt = 0;
    GLuint m_texture;
    std::vector<std::string>quiltFileNames;
};


void SimpleWindow::onInit(){
  ___;
  ge::gl::init((ge::gl::GET_PROC_ADDRESS)eglGetProcAddress);
  ___;
  std::cerr << ge::gl::glGetString(GL_VERSION) << std::endl;

  vars.addFloat      ("quiltView.pitch"      ,673.6839f);
  vars.addFloat      ("quiltView.tilt"       ,-0.0719594f);
  vars.addFloat      ("quiltView.center"     ,0.4528985f);
  vars.addFloat      ("quiltView.invView"    ,1.00f);
  vars.addFloat      ("quiltView.subp"       ,1.f/(1920*2*3.f));
  vars.add<glm::vec4>("quiltView.tile"       ,5.00f, 9.00f, 45.00f, 45.00f);
  vars.add<glm::vec4>("quiltView.viewPortion",0.99976f, 0.99976f, 0.00f, 0.00f);
  vars.addFloat      ("quiltView.focus"      ,-0.1f);
  //addVarsLimitsF(vars,"quiltView.focus",-1,+1,0.001f);
  //addVarsLimitsF(vars,"quiltView.tilt",-10,10,0.01);

  vars.addFloat("quiltRender.size",5.f);
  vars.addFloat("quiltRender.fov",90.f);
  vars.addFloat("quiltRender.viewCone",10.f);
  vars.addFloat("quiltRender.texScale",1.64f);
  //addVarsLimitsF(vars,"quiltRender.texScale",0.1f,5,0.01f);
  vars.addFloat("quiltRender.texScaleAspect",0.745f);
  //addVarsLimitsF(vars,"quiltRender.texScaleAspect",0.1f,10,0.01f);

  quiltFileNames = getDirFiles(argv[1]);
  quiltImage = std::make_shared<QuiltImage>(quiltFileNames.at(selectedQuilt),vars);
}

void SimpleWindow::onDraw(){
  ___;
  Timer<float>timer;
  timer.reset();
  ___;

  glDisable(GL_SCISSOR_TEST);
  ___;
  //ge::gl::glViewport(0,0,1920*2,1080*2);
  ge::gl::glClearColor(0,0,0,1);
  ge::gl::glClear(GL_COLOR_BUFFER_BIT);
  ___;

  quiltImage->draw();
  ___;
  ge::gl::glFinish();
  ___;

  eglSurface.swap();
  ___;

  auto t = timer.elapsedFromStart();
  std::cerr << "frame time: " << t << " - fps: " << 1/t << std::endl;
}

void SimpleWindow::onKeyDown(uint32_t k){
  std::cerr << "klavesa: " << k << std::endl;
  if(k == KEY_LEFT){
    quiltImage->decViewport();
    //glDisable(GL_SCISSOR_TEST);
    //ge::gl::glClearColor(0,0,0,1);
    //ge::gl::glClear(GL_COLOR_BUFFER_BIT);
  }
  if(k == KEY_RIGHT){
    //glDisable(GL_SCISSOR_TEST);
    //ge::gl::glClearColor(0,0,0,1);
    //ge::gl::glClear(GL_COLOR_BUFFER_BIT);
    quiltImage->incViewport();
  }
  if(k == KEY_UP){
    quiltImage->incFocus();
  }
  if(k == KEY_DOWN){
    quiltImage->decFocus();
  }
  if(k == KEY_N){
    ___;
    selectedQuilt++;
    if(selectedQuilt >= quiltFileNames.size())selectedQuilt = 0;
    quiltImage = std::make_shared<QuiltImage>(quiltFileNames.at(selectedQuilt),vars);
    ___;
  }
  if(k == KEY_P){
    ___;
    selectedQuilt--;
    if(selectedQuilt >= quiltFileNames.size())selectedQuilt = 0;
    quiltImage = std::make_shared<QuiltImage>(quiltFileNames.at(selectedQuilt),vars);
    ___;
  }
}


int main(int argc,char*argv[])
{
  auto win = SimpleWindow(1920*2,1080*2,true,argc,argv);
  //auto win = SimpleWindow(800,600,false);
  win.start();

  return EXIT_SUCCESS;
}
