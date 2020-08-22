#include <SDL.h>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <ArgumentViewer/ArgumentViewer.h>
#include <iostream>
#include <Simple3DApp/Application.h>
#include <Vars/Vars.h>
#include <VarsGLMDecorator/VarsGLMDecorator.h>
#include <imguiVars/imguiVars.h>
#include <imguiVars/addVarsLimits.h>
#include <Barrier.h>
#include <fileWatcher.hpp>

#include <FunctionPrologue.h>
#include<thread>
#include <keyframes.hpp>
#include <loadTxtFile.hpp>
#include <saveTxtFile.hpp>
#include <createDrawProgram.hpp>
#include <drawFinalFrame.hpp>
#include <Project.hpp>
#include <VideoManager.hpp>
#include <getTimeFormat.hpp>

#include <DVars.hpp>
#include <computeFrame.hpp>
#include <editProgram.hpp>
#include <loadFragment.hpp>
#include <mainMenu.hpp>
#include <onKey.hpp>
#include <parseArgs.hpp>
#include <init.hpp>



#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl


class RemSub: public simple3DApp::Application{
 public:
  RemSub(int argc, char* argv[]) : Application(argc, argv,330) {}
  virtual ~RemSub(){}
  virtual void draw() override;
  DVars vars;

  virtual void                init() override;
  virtual void                mouseMove(SDL_Event const& event) override;
  virtual void                key(SDL_Event const& e, bool down) override;
  virtual void                resize(uint32_t x,uint32_t y) override;
  virtual void                mouseWheel(SDL_Event const& event);
};




void RemSub::init(){
  window->setSize(1920,1080);
  *vars.add<std::shared_ptr<sdl2cpp::Window>>("window") = window;
  window->setEventCallback(SDL_MOUSEWHEEL,[&](SDL_Event const&e){mouseWheel(e);return true;});
  vars.addUVec2("windowSize",window->getWidth(),window->getHeight());

  parseArgs(vars,argc,argv);

  ::init(vars);
}





void RemSub::draw(){
  FUNCTION_CALLER();


  ___;


  auto proj = vars.get<Project>("project");
  proj->videoManager.readAndUploadIfNecessary();
  if(vars.addOrGetBool("playVideo",true)){
    proj->videoManager.nextFrame();
  }
  ___;

  vars.addOrGetInt32("frameId") = proj->videoManager.frame;
  ___;
  vars.addOrGetString("time") = getTimeFormat(proj->videoManager.frame,proj->videoManager.getFPS());
  ___;

  computeFrame(vars);
  ___;
  ge::gl::glMemoryBarrier(GL_ALL_BARRIER_BITS);
  drawFinalFrame(vars);
  ___;

  if(vars.addOrGetBool("saveMode",false)){
    proj->videoManager.saveFrameParallel();
#if defined(USE_OPENCV)
    if(proj->videoManager.frame >= proj->videoManager.getNofFrames() || proj->videoManager.streams.at(0).video->frame.empty())
#else
    if(proj->videoManager.frame >= proj->videoManager.getNofFrames())// || proj->videoManager.streams.at(0).video->frame.empty())
#endif
      exit(0);
  }
  ___;

  editProgram(vars);
  mainMenu(vars);
  ___;

  drawImguiVars(vars);
  ___;

  swap();
}

void RemSub::key(SDL_Event const& event, bool DOWN) {
  onKey(vars,event,DOWN);
}

void RemSub::mouseWheel(SDL_Event const& event){
  auto&zoom = vars.getFloat("view.zoom");
  auto zoomInc = 0.1f*event.wheel.y;
  int x,y;
  SDL_GetMouseState(&x,&y);
  float mx = 2.f*(      (float)x / (float)window->getWidth ()) - 1.f;
  float my = 2.f*(1.f - (float)y / (float)window->getHeight()) - 1.f;

  glm::vec2 mouse = glm::vec2(mx,my);
  auto&offset = vars.getVec2("view.offset");
  //T+S*B + offset-mouse
  //(m-o)/z = (m-o+d)/(z+i);
  //(m-o)*(z+i) = (m-o+d)*z
  //mz-oz+mi-oi = mz-oz+dz
  //mi-oi=dz
  //(m-o)*i = dz
  //(m-o)*i/z = d
  offset -= (mouse-offset)*zoomInc/zoom;


  zoom += zoomInc;
  if(zoom < 0.1f)zoom = 0.1f;
}

void RemSub::mouseMove(SDL_Event const& e) {
  if(e.motion.state & SDL_BUTTON_MMASK){
    auto&offset = vars.getVec2("view.offset");
    offset.x += 2.f * (float)e.motion.xrel / (float)window->getWidth();
    offset.y -= 2.f * (float)e.motion.yrel / (float)window->getHeight();
  }
}

void RemSub::resize(uint32_t x,uint32_t y){
  auto&windowSize = vars.getUVec2("windowSize");
  windowSize.x = x;
  windowSize.y = y;
  vars.updateTicks("windowSize");
  ge::gl::glViewport(0,0,x,y);
}


int main(int argc,char*argv[]){
  RemSub app{argc, argv};
  app.start();
  return EXIT_SUCCESS;
}

