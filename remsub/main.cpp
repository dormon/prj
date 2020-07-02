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
#include<thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
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
      INT   ,
      FLOAT ,
      DOUBLE,
    }type;
    union Value{
      int    i32;
      float  f32;
      double f64;
      Value(float  v):f32(v){}
      Value(int    v):i32(v){}
      Value(double v):f64(v){}
    }value;
    void set(int    v){value = v;}
    void set(float  v){value = v;}
    void set(double v){value = v;}
    Data(float  v):type(FLOAT ),value(v){}
    Data(int    v):type(INT   ),value(v){}
    Data(double v):type(DOUBLE),value(v){}
    template<typename T>T get()const;
    template<typename T>T&get();
    void print()const{
      if(type == INT   )std::cout << value.i32;
      if(type == FLOAT )std::cout << value.f32;
      if(type == DOUBLE)std::cout << value.f64;
    }
};
template<>int    Data::get<int   >()const{return value.i32;}
template<>float  Data::get<float >()const{return value.f32;}
template<>double Data::get<double>()const{return value.f64;}
template<>int   &Data::get<int   >()     {return value.i32;}
template<>float &Data::get<float >()     {return value.f32;}
template<>double&Data::get<double>()     {return value.f64;}


class Keyframe{
  public:
    std::map<std::string,Data>data;
    Keyframe(std::string const&n,int    v){set(n,v);}
    Keyframe(std::string const&n,float  v){set(n,v);}
    Keyframe(std::string const&n,double v){set(n,v);}

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
    T&get(std::string const&n,T&v){
      auto it = data.find(n);
      if(it == std::end(data))return v;//throw std::runtime_error("Keyframe::get("+n+") - no such value");
      return it->second.get<T>();
    }
    bool has(std::string const&n)const{
      return data.count(n) != 0;
    }
    void erase(std::string const&n){
      data.erase(n);
    }
    void print(int indent)const{
      for(auto const&d:data){
        for(int i=0;i<indent;++i)std::cout << " ";
        std::cout << d.first << ": ";
        d.second.print();
        std::cout <<  std::endl;
      }
    }
};

class Keyframes{
  public:
    std::map<int,Keyframe>keyframes;

    void print()const{
      for(auto const&key:keyframes){
        std::cout << "frame " << key.first << ": " << std::endl;
        key.second.print(2);
      }
    }
    template<typename T>
    static void moveToNext(T&it,T const&end,std::string const&n){
      it++;
      while(it!=end&&!it->second.has(n))it++;
    }
    template<typename T>
    static T find(T&it,T const& end,int f,std::string const&n){
      if(it == end)return end;
      auto prev = it;
      moveToNext(it,end,n);
      while(it != end){
        if(prev->first <= f && f < it->first && prev->second.has(n))
          return prev;
        prev = it;
        moveToNext(it,end,n);
      }
      if(prev->first <= f && prev->second.has(n))
        return prev;
      return end;
    }
    
    auto getKeyframe(int f,std::string const&n){
      auto       it   = std::begin(keyframes);
      auto const end  = std::end  (keyframes);
      return find(it,end,f,n);
    }
    auto const getKeyframe(int f,std::string const&n)const{
      auto       it   = std::cbegin(keyframes);
      auto const end  = std::cend  (keyframes);
      return find(it,end,f,n);
    }


    template<typename T>
    T get(int f,std::string const&n,T v)const{
      auto const it = getKeyframe(f,n);
      if(it == std::end(keyframes))return v;
      return it->second.get<T>(n,v);
    }

    template<typename T>
    T&get(int f,std::string const&n,T&v){
      auto it = getKeyframe(f,n);
      if(it == std::end(keyframes))return v;
      return it->second.get<T>(n);
    }

    template<typename T>
    void set(int f,std::string const&n,T v){
      std::cerr << "set("<<f<<","<<n<<","<<v<<")"<<std::endl;
      auto w = keyframes.emplace(std::piecewise_construct,std::forward_as_tuple(f),std::forward_as_tuple(n,v));
      if(!std::get<1>(w)){
        std::get<0>(w)->second.set(n,v);
      }
    }
    void erase(int f,std::string const&n){
      std::cerr <<"erase("<<f<<","<<n<<")"<<std::endl;
      auto it = getKeyframe(f,n);
      if(it == std::end(keyframes))return;
      auto&key = it->second;
      key.erase(n);
      if(key.data.empty())
        keyframes.erase(f);
    }

};

}

class VideoManager{
  public:
    struct Stream{
      float   fps         = 25.f     ;
      float   offset      = 0        ;
      int32_t start       = 0        ;
      int32_t end         = 0        ;
      int32_t nofFrames   = 0        ;
      int32_t loadedFrame = -10000000;
      std::shared_ptr<Video>video;
      std::shared_ptr<ge::gl::Texture>tex;
      std::string file;
      keyframe::Keyframes keys;
      Stream(std::string const&name):file(name){
        video = std::make_shared<Video>(name);
        tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
        tex->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        tex->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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

        float mainClipTime = frame/getFPS();

        auto timeInClip = mainClipTime-s.getOffset(frame);
        auto frameInClip = (int)roundf(timeInClip*s.fps);
        if(frameInClip <  0            )continue;
        if(frameInClip >= s.nofFrames  )continue;
        if(frameInClip == s.loadedFrame)continue;
        if(frameInClip != s.loadedFrame+1){
          s.video->moveToFrame(frameInClip);
        }

        s.video->readFrame();
        s.loadedFrame = frameInClip;
        if(!s.video->frame.empty())
          s.tex->setData2D(s.video->frame.data,GL_BGR_INTEGER);
      }

    }
    float getFPS(){
      if(streams.empty())return 25.f;
      return streams.at(0).fps;
    }
    void bind(){
      for(size_t i=0;i<streams.size();++i){
        auto&s = streams.at(i);
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
      finalFrame = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGBA8UI,1,v->width,v->height);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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

    template<typename ValueType>
    void showVarGUI(Stream&st,std::string const&var,ValueType step,ValueType mmin,ValueType mmax){
      std::stringstream addName;
      addName << "add " << var; 
      if(ImGui::Button(addName.str().c_str())){
        st.keys.set(frame,var.c_str(),0);
      }
      int counter = 0;

      std::vector<std::tuple<int,ValueType>>toInsert;
      std::vector<int>toErase;
      for(auto&key:st.keys.keyframes){
        if(!key.second.has(var))continue;
        ValueType defValue = 0;
        auto&value = key.second.get(var,defValue);
        int f = key.first;


        std::stringstream ss;
        ss << var << counter;

        if(ImGui::TreeNode(ss.str().c_str())){

          ImGui::InputInt("frame",&f);
          if(f != key.first){
            toErase.push_back(key.first);
            toInsert.push_back(std::tuple<int,int>(f,value));
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

          showVarGUI<float>(st,"offset"  ,0.0001f,-1e10,+1e10);
          showVarGUI<float>(st,"contrast",0.001f,0.001,10.f);

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

void createProgram(vars::Vars&vars){
  FUNCTION_PROLOGUE("remsub","computeSource");

  std::cerr << "createProgram" << std::endl;

  auto mpSrc = vars.getString("computeSource");
  auto prg = vars.reCreate<ge::gl::Program>(
      "program",
      std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER,mpSrc)
      );
  prg->setNonexistingUniformWarning(false);
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
      case GL_BOOL:
        {
          bool v;
          ge::gl::glGetUniformiv(prg->getId(),prg->getUniformLocation(name),(int*)&v);
          vars.addBool ("uniforms."+name,v);break;
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

  layout(binding=0)uniform usampler2D finalFrame;

  uniform uvec2 windowSize = uvec2(512);
  uniform float zoom       = 1.f;
  uniform vec2 offset      = vec2(0);

  out vec2 vCoord;

  void main(){
    ivec2 frameSize = textureSize(finalFrame,0).xy;

    vec2 scale = zoom*vec2(frameSize) / vec2(windowSize);

    vCoord = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4((-1+2*vCoord)*scale+offset,0,1);
    vCoord.y = 1-vCoord.y;
  }

  ).";

  auto fsSrc = R".(
  #version 450

  layout(binding=0)uniform usampler2D finalFrame;

  in vec2 vCoord;
  out vec3 fColor;

  void main(){
    fColor = vec3(texture(finalFrame,vCoord).xyz/vec3(255));
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
  ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), flags);
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
    if(proj->videoManager.frame >= proj->videoManager.getNofFrames())
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

