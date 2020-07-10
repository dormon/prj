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

#include <iomanip>
#include <fstream>


#include <FunctionPrologue.h>
#include<thread>
#include <keyframes.hpp>
#include <loadTxtFile.hpp>
#include <saveTxtFile.hpp>
#include <createDrawProgram.hpp>
#include <createComputeProgram.hpp>
#include <drawFinalFrame.hpp>
#include <Project.hpp>
#include <VideoManager.hpp>



#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl





using DVars = VarsGLMDecorator<vars::Vars>;

class EmptyProject: public simple3DApp::Application{
 public:
  EmptyProject(int argc, char* argv[]) : Application(argc, argv,330) {}
  virtual ~EmptyProject(){}
  virtual void draw() override;
  DVars vars;

  virtual void                init() override;
  virtual void                mouseMove(SDL_Event const& event) override;
  virtual void                key(SDL_Event const& e, bool down) override;
  virtual void                resize(uint32_t x,uint32_t y) override;
  virtual void                mouseWheel(SDL_Event const& event);
};

void loadFragment(vars::Vars&vars){
  FUNCTION_CALLER();
  *vars.reCreate<std::string>("computeSource") = loadTxtFile(vars.getString("computeSourceFile"));
}

void saveFragment(vars::Vars&vars){
  FUNCTION_CALLER();
  auto fn = vars.getString("computeSourceFile");
  auto f = std::ofstream(fn);
  auto data = vars.getString("computeSource");
  f.write(data.data(),data.size());
  f.close();
}



void EmptyProject::init(){
  window->setSize(1920,1080);
  *vars.add<std::shared_ptr<sdl2cpp::Window>>("window") = window;
  window->setEventCallback(SDL_MOUSEWHEEL,[&](SDL_Event const&e){mouseWheel(e);return true;});

  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto projectName  = args->gets("--proj","","project file");
  auto printHelp  = args->isPresent("--help");
  if(printHelp||!args->validate()){
    std::cerr <<args->toStr() << std::endl;
  }

  auto proj = vars.add<Project>("project");
  vars.addString("projectName",projectName);
  proj->load(projectName);

  vars.add<ge::gl::VertexArray>("emptyVao");
  vars.addUVec2("windowSize",window->getWidth(),window->getHeight());
  vars.addMap<SDL_Keycode, bool>("input.keyDown");

  vars.addBool("compile");

  vars.addString("computeSourceFile","../drawVideo.fp");
  loadFragment(vars);

  vars.addFloat("view.zoom",1.f);
  vars.add<glm::vec2>("view.offset",glm::vec2(0.f));
}

std::string getTimeFormat(size_t frameId,size_t fps){
  auto frame    = (frameId                ) % fps;
  auto second   = (frameId / fps          ) % 60;
  auto minute   = (frameId / fps / 60     ) % 60;
  auto hour     = (frameId / fps / 60 / 60);
  auto mili = (uint32_t)((float)frame / (float)fps * 1000);

  std::stringstream ss;

  ss << std::setfill('0') << std::setw(2) << hour   << ":";
  ss << std::setfill('0') << std::setw(2) << minute << ":";
  ss << std::setfill('0') << std::setw(2) << second << ":";
  ss << std::setfill('0') << std::setw(3) << mili;
  
  return ss.str();
}


void editProgram(vars::Vars&vars){
  FUNCTION_CALLER();

  auto&src = vars.getString("computeSource");
  static char text[1024*100];
  std::strcpy(text,src.data());

  static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
  ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 40), flags);
  src = text;

  ImGui::Columns(3,"col",false);
  if(ImGui::Button("comp")){
    vars.updateTicks("computeSource");
  }
  ImGui::NextColumn();
  if(ImGui::Button("save")){
    saveFragment(vars);
  }
  ImGui::NextColumn();
  if(ImGui::Button("load")){
    loadFragment(vars);
  }
  ImGui::Columns(1);

  auto proj = vars.get<Project>("project");
  proj->videoManager.setFrame();
  proj->videoManager.showGUI();

  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("file")){
      if(ImGui::MenuItem("open")){
      }
      if(ImGui::MenuItem("save")){
        vars.get<Project>("project")->save(vars.getString("projectName"));
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void createAuxBuffer(DVars&vars){
  FUNCTION_PROLOGUE("remsub");
  vars.reCreate<ge::gl::Buffer>("auxBuffer",sizeof(uint32_t)*1000);
}

void computeFrame(DVars&vars){
  FUNCTION_CALLER();

  createComputeProgram(vars);
  createAuxBuffer(vars);
  auto prg = vars.get<ge::gl::Program>("program");
  std::vector<std::string>uniformNames;
  vars.getDir(uniformNames,"uniforms");
  for(auto const&n:uniformNames){
    auto vName = "uniforms."+n;
    auto const&type = vars.getType(vName);
    if(type == typeid(bool    ))prg->set1i (n,vars.getBool  (vName));
    if(type == typeid(uint32_t))prg->set1ui(n,vars.getUint32(vName));
    if(type == typeid( int32_t))prg->set1i (n,vars.getInt32 (vName));
    if(type == typeid(float   ))prg->set1f (n,vars.getFloat (vName));
    if(type == typeid(glm::vec2))prg->set2fv(n,(float*)&vars.getVec2(vName));
  }
  auto proj = vars.get<Project>("project");
  proj->videoManager.bind();
  proj->videoManager.finalFrame->bindImage(0);
  prg->set1ui("activeClip",proj->videoManager.activeClip);
  proj->videoManager.setContrast(&*prg);
  proj->videoManager.setOffsetScale(&*prg);
  proj->videoManager.setSubBox(&*prg);

  auto auxBuffer = vars.get<ge::gl::Buffer>("auxBuffer");
  auxBuffer->clear(GL_R32UI,GL_RED_INTEGER,GL_UNSIGNED_INT);
  prg->bindBuffer("AuxBuffer",auxBuffer);
  prg->use();

  ge::gl::glDispatchCompute(vars.addOrGetInt32("wg",256),1,1);
}


void EmptyProject::draw(){
  FUNCTION_CALLER();




  auto proj = vars.get<Project>("project");
  proj->videoManager.readAndUploadIfNecessary();
  if(vars.addOrGetBool("playVideo",true)){
    proj->videoManager.nextFrame();
  }

  vars.addOrGetInt32("frameId") = proj->videoManager.frame;
  vars.addOrGetString("time") = getTimeFormat(proj->videoManager.frame,proj->videoManager.streams.at(0).video->fps);

  computeFrame(vars);
  ge::gl::glMemoryBarrier(GL_ALL_BARRIER_BITS);
  drawFinalFrame(vars);

  if(vars.addOrGetBool("saveMode",false)){
    proj->videoManager.saveFrameParallel();
#if defined(USE_OPENCV)
    if(proj->videoManager.frame >= proj->videoManager.getNofFrames() || proj->videoManager.streams.at(0).video->frame.empty())
#else
    if(proj->videoManager.frame >= proj->videoManager.getNofFrames())// || proj->videoManager.streams.at(0).video->frame.empty())
#endif
      exit(0);
  }

  editProgram(vars);

  drawImguiVars(vars);

  swap();
}

void EmptyProject::key(SDL_Event const& event, bool DOWN) {
  auto&keys = vars.getMap<SDL_Keycode, bool>("input.keyDown");
  keys[event.key.keysym.sym] = DOWN;
  if(DOWN && event.key.keysym.sym == SDLK_SPACE)
    vars.getBool("playVideo") ^= true;
  if(DOWN && event.key.keysym.sym == SDLK_RIGHT){
    vars.get<VideoManager>("videoManager")->nextFrame();
  }
  if(DOWN && event.key.keysym.sym == SDLK_LEFT){
    vars.get<VideoManager>("videoManager")->prevFrame();
  }

}

void EmptyProject::mouseWheel(SDL_Event const& event){
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

void EmptyProject::mouseMove(SDL_Event const& e) {
  if(e.motion.state & SDL_BUTTON_MMASK){
    auto&offset = vars.getVec2("view.offset");
    offset.x += 2.f * (float)e.motion.xrel / (float)window->getWidth();
    offset.y -= 2.f * (float)e.motion.yrel / (float)window->getHeight();
  }
}

void EmptyProject::resize(uint32_t x,uint32_t y){
  auto&windowSize = vars.getUVec2("windowSize");
  windowSize.x = x;
  windowSize.y = y;
  vars.updateTicks("windowSize");
  ge::gl::glViewport(0,0,x,y);
}


int main(int argc,char*argv[]){
  EmptyProject app{argc, argv};
  app.start();
  return EXIT_SUCCESS;
}

