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
#include <video.hpp>
#include <FunctionPrologue.h>
#include <json.hpp>
using namespace nlohmann;


#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl

std::string loadTxtFile(std::string const&fileName){
  auto file = std::ifstream(fileName);
  if(!file.is_open())throw std::runtime_error("loadTxtFile - cannot open "+fileName);
  std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
  return str;
}

void saveTxtFile(std::string const&fileName,std::string const&txt){
  auto file = std::ofstream(fileName);
  if(!file.is_open())throw std::runtime_error("saveTxtFile - cannot open "+fileName);
  file.write(txt.data(),txt.size());
  file.close();
}

namespace keyframe{

class Data{
  public:
    enum Type{
      INT  ,
      FLOAT,
    }type;
    union Value{
      int   i32;
      float f32;
      Value(float v):f32(v){}
      Value(int   v):i32(v){}
    }value;
    void set(int   v){value = v;}
    void set(float v){value = v;}
    Data(float v):type(FLOAT),value(v){}
    Data(int   v):type(INT  ),value(v){}
    template<typename T>T get()const;
    template<typename T>T&get();
};
template<>int   Data::get<int  >()const{return value.i32;}
template<>float Data::get<float>()const{return value.f32;}
template<>int  &Data::get<int  >()     {return value.i32;}
template<>float&Data::get<float>()     {return value.f32;}

class Keyframe{
  public:
    std::map<std::string,Data>data;
    Keyframe(std::string const&n,int   v){set(n,v);}
    Keyframe(std::string const&n,float v){set(n,v);}

    template<typename T>
    void set(std::string const&n,T const&v){
      auto w = data.emplace(n,v);
      if(!std::get<1>(w)){
        std::get<0>(w)->second.set(v);
      }
    }
    template<typename T>
    T get(std::string const&n,T v)const{
      auto it = data.find(n);
      if(it == std::end(data))return v;
      return it->second.get<T>();
    }
    template<typename T>
    T&get(std::string const&n){
      auto it = data.find(n);
      if(it == std::end(data))throw std::runtime_error("Keyframe::get("+n+") - no such value");
      return it->second.get<T>();
    }
    void erase(std::string const&n){
      data.erase(n);
    }
};

class Keyframes{
  public:
    std::map<int,Keyframe>keyframes;
    template<typename T>
    T get(int f,std::string const&n,T v)const{
      if(keyframes.empty())return v;
      return getKeyframe(f).get<T>(n,v);
    }
    template<typename T>
    T&get(int f,std::string const&n){
      return getKeyframe(f).get<T>(n);
    }
    template<typename T>
    void set(int f,std::string const&n,T v){
      auto w = keyframes.emplace(std::piecewise_construct,std::forward_as_tuple(f),std::forward_as_tuple(n,v));
      if(!std::get<1>(w)){
        std::get<0>(w)->second.set(n,v);
      }
    }
    void erase(int f,std::string const&n){
      auto key = getKeyframe(f);
      key.erase(n);
      if(key.data.empty())
        keyframes.erase(f);
    }

    Keyframe const&getKeyframe(int f)const{
      size_t i;
      auto it  = std::cbegin(keyframes);
      auto end = std::cend(keyframes);
      auto prev = it;
      it++;
      while(it != end){
        if(prev->first <= f && f < it->first)break;
        prev = it;
        it++;
      }
      return prev->second;
    }
    Keyframe &getKeyframe(int f){
      size_t i;
      auto it  = std::begin(keyframes);
      auto end = std::end(keyframes);
      auto prev = it;
      it++;
      while(it != end){
        if(prev->first <= f && f < it->first)break;
        prev = it;
        it++;
      }
      return prev->second;
    }
};

}

class VideoManager{
  public:
    struct Stream{
      int32_t offset = 0;
      int32_t start  = 0;
      int32_t end    = 0;
      int32_t frame  = 0;
      int32_t nofFrames = 0;
      int32_t loadedFrame = -10000000;
      std::shared_ptr<Video>video;
      std::shared_ptr<ge::gl::Texture>tex;
      std::string file;
      keyframe::Keyframes keys;
      Stream(std::string const&name):file(name){
        video = std::make_shared<Video>(name);
        tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
        nofFrames = video->nofFrames;
        end = video->nofFrames;
        keys.set<int>(0,"offset",0);
      }
      int getOffset(int f)const{
        return keys.get<int>(f,"offset",0);
      }
    };
    std::shared_ptr<ge::gl::Texture>finalFrame;
    int32_t frame;
    std::vector<Stream>streams;
    void readAndUploadIfNecessary(){
      for(auto&s:streams){
        if(!s.video)return;
        auto frameToRead = frame+s.getOffset(frame);//s.offset;
        if(frameToRead >= s.nofFrames  )continue;
        if(frameToRead == s.loadedFrame)continue;
        if(frameToRead >= s.end        )continue;
        if(frameToRead <  s.start      )continue;
        if(frameToRead != s.loadedFrame+1){
          s.video->moveToFrame(frameToRead);
        }

        s.video->readFrame();
        s.loadedFrame = frameToRead;
        s.frame = s.loadedFrame+1;
        s.tex->setData2D(s.video->frame.data,GL_BGR_INTEGER);
      }
    }
    void bind(){
      for(size_t i=0;i<streams.size();++i){
        auto&s = streams.at(i);
        if(!s.tex)continue;
        s.tex->bind(i);
      }
    }
    void nextFrame(){
      frame++;
    }
    void prevFrame(){
      frame--;
      if(frame<0)frame=0;
    }
    void gotoFrame(int32_t f){
      frame = f;
    }
    void createFinalFrame(){
      if(streams.size() != 1)return;
      auto const&v = streams.at(0).video;
      finalFrame = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGBA32F,1,v->width,v->height);
    }
    void addVideo(std::string const&n){
      streams.emplace_back(n);
      createFinalFrame();
    }
    void setOffset(){
      for(size_t i=0;i<streams.size();++i){
        auto&s = streams.at(i);
        std::stringstream ss;
        ss << "stream"<<i<<" offset";
        auto name = ss.str();
        ImGui::InputInt(name.c_str(),&s.offset);
      }
    }

    int selectedStream = 1;
    void addOffsetGUI(){
      int f       = frame;

      ImGui::Columns(3,"col",false);
      ImGui::InputInt("stream",&selectedStream);
      if(selectedStream >= streams.size())selectedStream = streams.size()-1;
      ImGui::NextColumn();
      auto&offset = streams.at(selectedStream).keys.get<int>(frame,"offset");
      ImGui::InputInt("offset",&offset);
      ImGui::NextColumn();
      if(ImGui::Button("add")){
        streams.at(selectedStream).keys.set(frame,"offset",offset);
      }
      ImGui::Columns(1);
    }
    void showOffsetsGUI(){
      auto&st = streams.at(selectedStream);
      int counter = 0;
      std::vector<std::tuple<int,int>>toInsert;
      std::vector<int>toErase;
      for(auto&key:st.keys.keyframes){
        auto it = key.second.data.find("offset");
        if(it == std::end(key.second.data))continue;
        auto&o = it->second.value.i32;
        int f = key.first;


        std::stringstream ss;
        ss << "o" << counter;

        if(ImGui::TreeNode(ss.str().c_str())){

          ImGui::InputInt("frame",&f);
          if(f != key.first){
            toErase.push_back(key.first);
            toInsert.push_back(std::tuple<int,int>(f,o));
          }
          
          ImGui::InputInt("offset",&o);

          ImGui::TreePop();
        }
        counter++;

      }
      for(auto const&t:toErase)
        st.keys.erase(t,"offset");
      for(auto const&oo:toInsert)
        st.keys.set(std::get<0>(oo),"offset",std::get<1>(oo));
    }
    void setFrame(){
      ImGui::InputInt("goto",&frame);
    }
};


class Project{
  public:
    void load(std::string const&file){
      auto j = json::parse(loadTxtFile(file));
      auto const&js = j.at("streams");
      auto nVideos = js.size();

      auto const loadKeyData = [&](keyframe::Keyframes&s,int frame,json const&j){
        for(auto const&it:j.items()){
          if(it.value().is_number_integer())s.set<int  >(frame,it.key(),it.value().get<int  >());
          if(it.value().is_number_float  ())s.set<float>(frame,it.key(),it.value().get<float>());
        }
      };
      auto const loadKey = [&](keyframe::Keyframes&s,json const&j){
        int frame = j.at("frame");
        loadKeyData(s,frame,j.at("data"));
      };

      auto const loadKeys = [&](keyframe::Keyframes&s,json const&j){
        if(j.count("keys")==0)return;
        for(size_t i=0;i<j.at("keys").size();++i)
          loadKey(s,j.at("keys").at(i));
      };

      for(size_t i=0;i<nVideos;++i){
        videoManager.addVideo(js.at(i).at("file"));
        loadKeys(videoManager.streams.back().keys,js.at(i));
      }
    }
    void save(std::string const&file){
      json j;
      size_t counter=0;
      auto const saveKeyData = [&](json&j,keyframe::Keyframe const&key){
        for(auto const&d:key.data){
          if(d.second.type == keyframe::Data::FLOAT)
            j[d.first] = d.second.value.f32;
          if(d.second.type == keyframe::Data::INT)
            j[d.first] = d.second.value.i32;
        }
      };

      auto const saveKey = [&](json&j,keyframe::Keyframe const&key,int frame){
        j["frame"] = frame;
        saveKeyData(j["data"],key);
      };

      auto const saveKeys = [&](json&j,keyframe::Keyframes const&keys){
        size_t keyCounter = 0;
        for(auto const&k:keys.keyframes){
          saveKey(j["keys"][keyCounter],k.second,k.first);
          keyCounter++;
        }
      };

      for(auto const&s:videoManager.streams){
        j["streams"][counter]["file"] = s.file;
        saveKeys(j["streams"][counter],s.keys);
        counter++;
      }

      saveTxtFile(file,j.dump(2));
    }
    VideoManager        videoManager;
};


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
  auto projName  = args->gets("--proj","","project file");
  auto printHelp  = args->isPresent("--help");
  if(printHelp||!args->validate()){
    std::cerr <<args->toStr() << std::endl;
  }

  auto proj = vars.add<Project>("project");
  proj->load(projName);

  vars.add<ge::gl::VertexArray>("emptyVao");
  vars.addUVec2("windowSize",window->getWidth(),window->getHeight());
  vars.addMap<SDL_Keycode, bool>("input.keyDown");

  vars.addBool("compile");

  vars.addString("computeSourceFile","../drawVideo.fp");
  loadFragment(vars);

  //vars.addBool("drawVideoFileWatcher");
  //auto watcher = vars.add<fileWatcher::FileWatcher>("fileWatcher");
  //watcher->watch("../drawVideo.fp",[&](){std::cerr << "jojo" << std::endl;vars.updateTicks("drawVideoFileWatcher");});
}


//void watchFiles(vars::Vars&vars){
//  auto watcher = vars.get<fileWatcher::FileWatcher>("fileWatcher");
//  watcher->operator()();
//}

void createProgram(vars::Vars&vars){
  FUNCTION_PROLOGUE("remsub","computeSource");

  std::cerr << "createProgram" << std::endl;

  auto mpSrc = vars.getString("computeSource");
  auto prg = vars.reCreate<ge::gl::Program>(
      "program",
      std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER,mpSrc)
      );
  auto info = prg->getInfo();
  vars.erase("uniforms");
  for(auto const&u:info->uniforms){
    auto const&name = u.first;
    if(name.find("[")!=std::string::npos)continue;
    auto const&unif = u.second;
    auto type = std::get<ge::gl::ProgramInfo::TYPE>(unif);
    switch(type){
      case GL_UNSIGNED_INT:
        {
          uint32_t v;
          ge::gl::glGetUniformuiv(prg->getId(),prg->getUniformLocation(name),&v);
          vars.addUint32("uniforms."+name,v);break;
        }
      case GL_INT_VEC2:
        {
          glm::ivec2 v;
          ge::gl::glGetUniformiv(prg->getId(),prg->getUniformLocation(name),(int*)&v);
          vars.add<glm::ivec2>("uniforms."+name,v);break;
        }
      case GL_FLOAT       :
        {
          float v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),&v);
          vars.addFloat ("uniforms."+name,v);break;
        }
      case GL_FLOAT_VEC2:
        {
          glm::vec2 v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),(float*)&v);
          vars.add<glm::vec2>("uniforms."+name,v);break;
        }
      default:break;


    }


  }
}

void createDrawProgram(vars::Vars&vars){
  FUNCTION_PROLOGUE("remsub");
  std::string const vsSrc = R".(
  #version 450

  layout(binding=0)uniform sampler2D finalFrame;

  uniform uvec2 windowSize = uvec2(512);
  uniform float zoom       = 1.f;
  uniform vec2 offset      = vec2(0);

  out vec2 vCoord;

  void main(){
    ivec2 frameSize = textureSize(finalFrame,0).xy;

    vec2 scale = zoom*vec2(frameSize) / vec2(windowSize);

    vCoord = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4((-1+2*vCoord)*scale+offset,0,1);
  }

  ).";

  auto fsSrc = R".(
  #version 450

  layout(binding=0)uniform sampler2D finalFrame;

  in vec2 vCoord;
  out vec3 fColor;

  void main(){
    fColor = texture(finalFrame,vCoord).xyz;
  }

  ).";
  auto prg = vars.reCreate<ge::gl::Program>(
      "drawProgram",
      std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER  ,vsSrc),
      std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,fsSrc)
      );
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
  ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 32), flags);
  src = text;


  auto proj = vars.get<Project>("project");
  proj->videoManager.setFrame();
  //proj->videoManager.setOffset();
  proj->videoManager.addOffsetGUI();
  proj->videoManager.showOffsetsGUI();

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
  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("file")){
      if(ImGui::MenuItem("open")){
      }
      if(ImGui::MenuItem("save")){
        vars.get<Project>("project")->save("../proj.json");
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

  createProgram(vars);
  createAuxBuffer(vars);
  auto prg = vars.get<ge::gl::Program>("program");
  std::vector<std::string>uniformNames;
  vars.getDir(uniformNames,"uniforms");
  for(auto const&n:uniformNames){
    auto vName = "uniforms."+n;
    auto const&type = vars.getType(vName);
    if(type == typeid(uint32_t))prg->set1ui(n,vars.getUint32(vName));
    if(type == typeid( int32_t))prg->set1i (n,vars.getInt32 (vName));
    if(type == typeid(float   ))prg->set1f (n,vars.getFloat (vName));
    if(type == typeid(glm::vec2))prg->set2fv(n,(float*)&vars.getVec2(vName));
  }
  auto proj = vars.get<Project>("project");
  proj->videoManager.bind();
  proj->videoManager.finalFrame->bindImage(0);

  auto auxBuffer = vars.get<ge::gl::Buffer>("auxBuffer");
  auxBuffer->clear(GL_R32UI,GL_RED_INTEGER,GL_UNSIGNED_INT);
  prg->bindBuffer("AuxBuffer",auxBuffer);
  prg->use();

  ge::gl::glDispatchCompute(vars.addOrGetInt32("wg",256),1,1);
}

void drawFinalFrame(DVars&vars){
  FUNCTION_CALLER();
  createDrawProgram(vars);
  ge::gl::glClearColor(0.1f,0.1f,0.1f,1.f);
  ge::gl::glClear(GL_COLOR_BUFFER_BIT);

  auto vao = vars.get<ge::gl::VertexArray>("emptyVao");
  auto prg = vars.get<ge::gl::Program>("drawProgram");

  vao->bind();
  auto proj = vars.get<Project>("project");
  proj->videoManager.finalFrame->bind(0);

  prg->use();
  prg->set2uiv("windowSize",(uint32_t*)&(vars.getUVec2("windowSize")));

  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);

  vao->unbind();
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
  auto&zoom = vars.getFloat("uniforms.zoom");
  auto zoomInc = 0.1f*event.wheel.y;
  int x,y;
  SDL_GetMouseState(&x,&y);
  float mx = 2.f*(      (float)x / (float)window->getWidth ()) - 1.f;
  float my = 2.f*(1.f - (float)y / (float)window->getHeight()) - 1.f;

  glm::vec2 mouse = glm::vec2(mx,my);
  auto&offset = vars.getVec2("uniforms.offset");
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
    auto&offset = vars.getVec2("uniforms.offset");
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

