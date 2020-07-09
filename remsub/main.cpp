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

#if defined(USE_OPENCV)
#include <video.hpp>
#else
#include <CachedVideo.hpp>
#endif

#include <FunctionPrologue.h>
#include <json.hpp>
#include<thread>
#include <keyframes.hpp>
#include <loadTxtFile.hpp>
#include <saveTxtFile.hpp>
#include <createDrawProgram.hpp>
#include <createComputeProgram.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
using namespace nlohmann;


#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl


class VideoManager{
  public:
    struct Stream{
      float   fps         = 25.f     ;
      float   offset      = 0        ;
      int32_t start       = 0        ;
      int32_t end         = 0        ;
      int32_t nofFrames   = 0        ;
      int32_t loadedFrame = -10000000;
#if defined(USE_OPENCV)
      std::shared_ptr<Video>video;
#else
      std::shared_ptr<cachedVideo::Video>video;
#endif
      std::shared_ptr<ge::gl::Texture>tex;
      std::string file;
      keyframe::Keyframes keys;
      Stream(std::string const&name):file(name){
#if defined(USE_OPENCV)
        video = std::make_shared<Video>(name.c_str());
#else
        video = std::make_shared<cachedVideo::Video>(name.c_str());
#endif
        tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
        tex->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        tex->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        tex->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        tex->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        nofFrames = video->nofFrames;
        end = video->nofFrames;
        fps = video->fps;
        //keys.set<int>(0,"offset",0);
        //keys.set<float>(0,"contrast",1);
      }
      float getOffset(int f)const{
        return keys.get<float>(f,"offset",0);
      }
      float getContrast(int f)const{
        return keys.get<float>(f,"contrast",1);
      }
      glm::vec4 getOffsetScale(int f)const{
        return keys.get<glm::vec4>(f,"offsetScale",glm::vec4(0.f,0.f,1.f,1.f));
      }
    };
    ~VideoManager(){
      for(auto&t:threads)
        if(t.joinable())t.join();
    }
    std::shared_ptr<ge::gl::Texture>finalFrame;
    int32_t frame;
    std::vector<Stream>streams;
    uint32_t activeClip = 0;

    void setContrast(ge::gl::Program*prg){
      std::vector<float>c;
      for(auto const&s:streams)
        c.push_back(s.getContrast(frame));
      prg->set1fv("contrast",c.data(),c.size());
    }
    void setOffsetScale(ge::gl::Program*prg){
      std::vector<glm::vec4>c;
      for(auto const&s:streams)
        c.push_back(s.getOffsetScale(frame));
      prg->set4fv("offsetScale",(float*)c.data(),c.size());
    }
    void setSubBox(ge::gl::Program*prg){
      if(streams.empty())return;
      auto c = streams.at(0).keys.get<glm::vec4>(frame,"subBox",glm::vec4(0.f,1.f,0.f,1.f));
      prg->set4fv("subBox",(float*)&c);
    }

    int32_t getNofFrames()const{
      if(streams.empty())return 0;
      return streams.at(0).nofFrames;
    }

    void readAndUploadIfNecessary(){
      activeClip = 0;
      int clipCounter = 0;
      for(size_t clipCounter=0;clipCounter<streams.size();++clipCounter){
        auto&s=streams.at(clipCounter);
        if(!s.video)return;


        if(frame   >= s.end        )continue;
        if(frame   <  s.start      )continue;


        activeClip |= 1<<clipCounter;



        int frameInClip;
        if(clipCounter == 0){
          frameInClip = frame;
        }else{
          float mainClipTime = frame/getFPS();
          auto timeInClip = mainClipTime-s.getOffset(frame);
          //std::cerr << "timeInClip: " << timeInClip << std::endl;
          int ss = (int)(timeInClip*s.fps);
          int se = ss+1;
          float sss = (float)ss / s.fps;
          float see = (float)se / s.fps;
          if(fabs(sss-timeInClip) < fabs(see-timeInClip))
            frameInClip = ss;
          else
            frameInClip = se;

          //frameInClip = (int)roundf(timeInClip*s.fps);
        }
        //if(clipCounter != 0){
        //  std::cerr << "frameTime: " << s.video->getFrameTime() << std::endl;
        //}
        //if(clipCounter == 0){
        //  std::cerr << "main: " << s.video->getFrameId() << " " << s.video->getFrameTime() << std::endl;
        //}

        if(frameInClip <  0            )continue;
        if(frameInClip >= s.nofFrames  )continue;
        if(frameInClip == s.video->getFrameId()-1)continue;
        //if(frameInClip != s.video->getFrameId()){
        //  s.video->moveToFrame(frameInClip);
        //}

        //s.video->readFrame();
        s.loadedFrame = frameInClip;
#if defined(USE_OPENCV)
        if(!s.video->frame.empty())
          s.tex->setData2D(s.video->frame.data,GL_BGR_INTEGER);
#else
        s.tex->setData2D(s.video->getFrame(frameInClip).data(),GL_BGR_INTEGER);
#endif
      }

    }
    float getFPS(){
      if(streams.empty())return 25.f;
      return streams.at(0).fps;
    }
    void bind(){
      for(size_t i=0;i<streams.size();++i){
        auto&s = streams.at(i);
        std::cerr << "bw: " << s.tex->getWidth(0) << std::endl;
        std::cerr << "bh: " << s.tex->getHeight(0) << std::endl;

        if(!s.tex)continue;
        s.tex->bind(i);
      }
    }
    void clampFrame(){
      int32_t n = getNofFrames();
      if(frame >= n)frame = n-1;
      if(frame < 0 )frame = 0;
    }
    void nextFrame(){
      frame++;
      clampFrame();
    }
    void prevFrame(){
      frame--;
      clampFrame();
    }
    void gotoFrame(int32_t f){
      frame = f;
      clampFrame();
    }

    struct CPUFrame{
      std::vector<uint8_t>data;
      int frame;
      CPUFrame(int s=1){
        data.resize(s);
      }
    };

    struct FrameBuffer{
      std::vector<CPUFrame>frames   ;
      std::vector<bool>    freeToUse;
      int w;
      int h;
      FrameBuffer(int w=1,int h=1,int n=1):w(w),h(h){
        freeToUse   = std::vector<bool>(n,true);
        frames      = std::vector<CPUFrame>(n,w*h*3);
      }
    };
    FrameBuffer frameBuffer;
    size_t nofThreads = 8;
    std::vector<std::thread>threads;


    void createFinalFrame(){
      if(streams.size() != 1)return;
      auto const&v = streams.at(0).video;
      std::cerr << "finalFrame: " << v->width << " x " << v->height << std::endl;
      finalFrame = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGBA8UI,1,v->width,v->height);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      finalFrame->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      finalFrame->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      int h = streams.at(0).video->height;
      int w = streams.at(0).video->width;
      frameBuffer = FrameBuffer(w,h,nofThreads);
      threads.resize(nofThreads);
    }


    void addVideo(std::string const&n){
      streams.emplace_back(n);
      createFinalFrame();
    }

    void setKeyValueGUI(std::string const&var,int&value,int step,int mmin,int mmax){
      ImGui::InputInt(var.c_str(),&value,step,mmin,mmax);
    }
    void setKeyValueGUI(std::string const&var,float&value,float step,float mmin,float mmax){
      ImGui::DragFloat(var.c_str(),&value,step,mmin,mmax,"%.7f");
    }
    void setKeyValueGUI(std::string const&var,glm::vec4&value,glm::vec4 const& step,glm::vec4 const& mmin,glm::vec4 const& mmax){
      ImGui::DragFloat4(var.c_str(),(float*)&value,step.x,mmin.x,mmax.x,"%.7f");
    }

    template<typename ValueType>
    void showVarGUI(Stream&st,std::string const&var,ValueType step,ValueType mmin,ValueType mmax){
      std::stringstream addName;
      addName << "add " << var; 
      if(ImGui::Button(addName.str().c_str())){
        st.keys.set<ValueType>(frame,var.c_str(),ValueType(0));
      }
      int counter = 0;

      std::vector<std::tuple<int,ValueType>>toInsert;
      std::vector<int>toErase;
      for(auto&key:st.keys.keyframes){
        if(!key.second.has(var))continue;
        ValueType defValue = ValueType(0);
        auto&value = key.second.get(var,defValue);
        int f = key.first;


        std::stringstream ss;
        ss << var << counter;

        if(ImGui::TreeNode(ss.str().c_str())){

          ImGui::InputInt("frame",&f);
          if(f != key.first){
            toErase.push_back(key.first);
            toInsert.push_back(std::tuple<int,ValueType>(f,value));
          }
     
          setKeyValueGUI(var,value,step,mmin,mmax);
          if(ImGui::Button("delete"))
            toErase.push_back(key.first);

          ImGui::TreePop();
        }
        counter++;

      }
      for(auto const&t:toErase)
        st.keys.erase(t,var.c_str());
      for(auto const&oo:toInsert)
        st.keys.set(std::get<0>(oo),var,std::get<1>(oo));
    }
    std::string outputExtension = "jpg";
    std::string outputName = "/media/paposud/hodhod/01/01_";
    void downloadFrame(std::vector<uint8_t>&data){
      ge::gl::glGetTextureImage(finalFrame->getId(),0,GL_RGB_INTEGER,GL_UNSIGNED_BYTE,data.size(),data.data());
    }
    void saveFrame(int frame,int w,int h,std::vector<uint8_t>&data){
      std::stringstream ss;
      ss << outputName << std::setfill('0') << std::setw(7) << frame << "."<<outputExtension;
      if(outputExtension == "jpg"){
        stbi_write_jpg(ss.str().c_str(),w,h,3,data.data(),100);
      }
      if(outputExtension == "png"){
        stbi_write_png(ss.str().c_str(),w,h,3,data.data(),3);
      }
    }

    int getWidth()const{
      return streams.at(0).video->width;
    }
    int getHeight()const{
      return streams.at(0).video->height;
    }



    void saveFrame(){
      downloadFrame(frameBuffer.frames.at(0).data);
      saveFrame(frame,getWidth(),getHeight(),frameBuffer.frames.at(0).data);
    }

    size_t findFreeThread(){
      size_t i = 0;

      while(true){
        i = 0;
        for(i=0;i<nofThreads;++i){
          if(frameBuffer.freeToUse.at(i)){
            if(threads.at(i).joinable())
              threads.at(i).join();
            frameBuffer.freeToUse.at(i) = false;
            break;
          }
        }
        if(i<frameBuffer.frames.size())
          break;
      }
      return i;
    }

    void saveFrameParallel(){
      auto i = findFreeThread();

      downloadFrame(frameBuffer.frames.at(i).data);

      threads.at(i) = std::thread([i,this](){
        saveFrame(this->frame,getWidth(),getHeight(),frameBuffer.frames.at(i).data);
        frameBuffer.freeToUse.at(i) = true;
      });
    }

    void guiTextInput(std::string const&name,std::string&txt){
      size_t const bufSize = 256;
      char buf[bufSize];
      strcpy(buf, txt.c_str());
      ImGui::InputText(name.c_str(),buf,bufSize);
      txt = std::string(buf);
    }

    void showGUI(){
      if(ImGui::Button("screen")){
        saveFrame();
      }
      guiTextInput("ext",outputExtension);
      guiTextInput("outputName",outputName);

      size_t streamCounter=0;
      for(auto&st:streams){
        std::stringstream ss;
        ss << st.file;
        if(ImGui::TreeNode(ss.str().c_str())){
          if(activeClip&(1<<streamCounter))
            ImGui::Text("active");
          else
            ImGui::Text("inactive");

          ImGui::InputInt("start",&st.start);
          ImGui::InputInt("end"  ,&st.end  );

          if(ImGui::Button("print keys")){
            st.keys.print();
          }

          showVarGUI<float    >(st,"offset"     ,0.0001f           ,-1e10           ,+1e10           );
          showVarGUI<float    >(st,"contrast"   ,0.001f            ,0.001           ,10.f            );
          showVarGUI<glm::vec4>(st,"offsetScale",glm::vec4(0.0001f),glm::vec4(-2.f) ,glm::vec4(2.f)  );
          if(streamCounter == 0)
            showVarGUI<glm::vec4>(st,"subBox",glm::vec4(0.01f),glm::vec4(0.f) ,glm::vec4(1.f)  );


          ImGui::TreePop();
        }
        streamCounter++;
      }
    }
    void setFrame(){
      ImGui::InputInt("goto",&frame);
      clampFrame();
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
          if(it.value().is_array()&&it.value().size() == 4&&it.value().at(0).is_number_float()){
            glm::vec4 v;
            for(int i=0;i<4;++i)
              v[i] = it.value().at(i).get<float>();
            s.set<glm::vec4>(frame,it.key(),v);
          }
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

      auto const loadStartEnd = [&](int&start,int&end,json const&j){
        if(j.count("start"))start = j["start"].get<int>();
        if(j.count("end"  ))end   = j["end"  ].get<int>();
      };

      for(size_t i=0;i<nVideos;++i){
        videoManager.addVideo(js.at(i).at("file"));
        auto&last = videoManager.streams.back();
        loadKeys(last.keys,js.at(i));
        loadStartEnd(last.start,last.end,js.at(i));
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
          if(d.second.type == keyframe::Data::DOUBLE)
            j[d.first] = d.second.value.f64;
          if(d.second.type == keyframe::Data::VEC4){
            for(int i=0;i<4;++i)
              j[d.first][i] = d.second.value.vec4[i];
          }
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

      auto const saveStartEnd = [&](json&j,int start,int end){
        j["start"] = start;
        j["end"  ] = end  ;
      };

      for(auto const&s:videoManager.streams){
        j["streams"][counter]["file"] = s.file;
        saveKeys    (j["streams"][counter],s.keys);
        saveStartEnd(j["streams"][counter],s.start,s.end);
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
  //vars.addBool("drawVideoFileWatcher");
  //auto watcher = vars.add<fileWatcher::FileWatcher>("fileWatcher");
  //watcher->watch("../drawVideo.fp",[&](){std::cerr << "jojo" << std::endl;vars.updateTicks("drawVideoFileWatcher");});
}


//void watchFiles(vars::Vars&vars){
//  auto watcher = vars.get<fileWatcher::FileWatcher>("fileWatcher");
//  watcher->operator()();
//}


//uniform vec2  help0offset         = vec2(-4,46);
//uniform vec2  help0scale          = vec2(10052,9856);


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
  std::cerr << "drawW: " << proj->videoManager.finalFrame->getWidth(0)<< std::endl;
  std::cerr << "drawH: " << proj->videoManager.finalFrame->getHeight(0)<< std::endl;

  prg->use();
  prg->set2uiv("windowSize",(uint32_t*)&(vars.getUVec2("windowSize")));
  prg->set1f("zoom",vars.getFloat("view.zoom"));
  prg->set2fv("offset",(float*)vars.get<glm::vec2>("view.offset"));

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

