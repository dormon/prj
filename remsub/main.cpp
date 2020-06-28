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



struct ivec2{
  int32_t x;
  int32_t y;
  ivec2(int32_t x = 0,int32_t y =0):x(x),y(y){}
  ivec2 operator+(ivec2 const&o)const{
    return ivec2(x+o.x,y+o.y);
  }
  ivec2 operator-(ivec2 const&o)const{
    return ivec2(x-o.x,y-o.y);
  }
  bool operator<(ivec2 const&o)const{
    if(x<o.x)return true;
    if(x>o.x)return false;
    return y<o.y;
  }
};

auto const writeColor = [](cv::Mat const&image,ivec2 const&coord,uint32_t color){
  for(uint32_t c=0;c<image.channels();++c)
    image.data[(image.cols*coord.y + coord.x)*image.channels() + c] = (color>>(c*8))&0xff;
};

template<typename T>
T min(T a,T b){
  return a<b?a:b;
}

int32_t max(int32_t a,int32_t b){
  return a>b?a:b;
}

#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl

class ImageLetter{
  public:
    ImageLetter(cv::Mat const&m,ivec2 const&coord,ivec2 const&size):img(m),coord(coord),size(size){
    }
    ~ImageLetter(){
    }
    cv::Mat img;
    ivec2 coord;
    ivec2 size;
    std::string name;
    bool operator<(ImageLetter const&o)const{
      return coord.x < o.coord.x;
    }
    void save()const{
      std::stringstream ss;
      ss << name << ".png";
      auto f = ss.str();
      std::cerr << f << std::endl;
      imwrite(f.c_str(),img);
    }
};

class Letter{
  public:
    std::map<ivec2,uint32_t>pixels;
    ivec2 minPixel;
    ivec2 maxPixel;
    void computeMinMax(){
      minPixel = ivec2(+1e8,+1e8);
      maxPixel = ivec2(-1e8,-1e8);
      for(auto const&x:pixels){
        auto const&c=x.first;
        //std::cerr << "c: " << c.x << " - " << c.y << std::endl;
        minPixel.x = min(minPixel.x,c.x);
        minPixel.y = min(minPixel.y,c.y);
        maxPixel.x = max(maxPixel.x,c.x);
        maxPixel.y = max(maxPixel.y,c.y);
      }
    }
    ImageLetter toImg(){
      computeMinMax();
      auto const w = maxPixel.x - minPixel.x + 1;
      auto const h = maxPixel.y - minPixel.y + 1;
      auto img = cv::Mat(h,w, CV_8UC4, cv::Scalar(0,0,0,0));
      for(auto const&x:pixels){
        auto const coord=x.first - minPixel;
        auto const&color = x.second;
        writeColor(img,coord,color);
      }
      return ImageLetter(img,minPixel,maxPixel-minPixel+ivec2(1,1));
    }
};

auto const readColor = [](cv::Mat const&image,ivec2 const&coord){
  uint32_t color = 0;
  for(uint32_t c=0;c<image.channels();++c)
    color |= image.data[(image.cols*coord.y + coord.x)*image.channels() + c] << c*8;
  //std::cerr << "0x" << std::hex << std::setw(8) << std::setfill('0') << color << std::endl;
  return color;
};

class Font{
  public:
    Font(std::string const&fileName){
      auto normalFont = imread(fileName.c_str(),cv::IMREAD_UNCHANGED);
      auto const width    = normalFont.cols;
      auto const height   = normalFont.rows;
      auto const channels = normalFont.channels();
      std::cerr << "font rows: " << width    << std::endl;
      std::cerr << "font cols: " << height   << std::endl;
      std::cerr << "font chan: " << channels << std::endl;

      std::set<ivec2>toExplore;
      for(uint32_t y=0;y<height;++y)
        for(uint32_t x=0;x<width;++x)
          toExplore.insert(ivec2(x,y));

      auto start = ivec2(0,0);



      auto const isBackground = [](uint32_t color){return (color>>(3*8)) == 0;};



      std::vector<Letter>letters;

      auto const removeBackground = [&](){
        for(uint32_t y=0;y<height;++y)
          for(uint32_t x=0;x<width;++x){
            auto coord = ivec2(x,y);
            if(isBackground(readColor(normalFont,coord))){
              toExplore.erase(toExplore.find(coord));
            }
          }
      };

      std::cerr << "toExplore: " << toExplore.size() << std::endl;
      removeBackground();
      std::cerr << "toExplore: " << toExplore.size() << std::endl;

      std::set<ivec2>toSearch;

      auto const insertToSearchIfPossible = [&](ivec2 const&newCoord){
        if(newCoord.x < 0 || newCoord.y < 0 || newCoord.x >= width || newCoord.y >= height || toExplore.find(newCoord) == toExplore.end())return;
        toSearch.insert(newCoord);
        toExplore.erase(toExplore.find(newCoord));
      };

      while(!toExplore.empty()){
        auto it = toExplore.begin();
        toExplore.erase(it);
        auto coord = *it;

        Letter letter;

        toSearch.insert(coord);
        while(!toSearch.empty()){
          auto it = toSearch.begin();
          auto c = *it;
          toSearch.erase(it);

          letter.pixels[c] = readColor(normalFont,c);
          for(int y=-10;y<20;++y)
            for(int x=-1;x<2;++x){
              if(x==y)continue;
              insertToSearchIfPossible(c+ivec2(x,y));
            }

        }

        letters.push_back(letter);
      }
      std::vector<ImageLetter>result;
      for(auto&l:letters)
        result.push_back(l.toImg());

      std::sort(std::begin(result),std::end(result));
      std::string const chars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
      for(size_t i=0;i<result.size()&&i<chars.size();++i)
        result[i].name = chars[i];
      lts = result;

      std::map<int32_t,int32_t>valueCount;
      for(auto const&l:lts)
        valueCount[l.coord.y]++;

      int32_t maxId = 0;
      int32_t maxCount = 0;
      for(auto const&vc:valueCount){
        if(vc.second > maxCount)base = vc.first;
      }
      
    }

    ImageLetter const&get(char l){
      return lts.at(l-'!');
    }

    int32_t base = 0;
    std::vector<ImageLetter>lts;
};

float colorDistance(uint32_t i,uint32_t r){
  float alpha = (float)(r>>(8*3))/(255.f);
  float dd[3];
  for(int k=0;k<3;++k){
    dd[k] = (float)((i>>(8*k))&0xff)/255.f - (float)((r>>(8*k))&0xff)/255.f;
    dd[k]*=dd[k];
  }
  return sqrt(dd[0]+dd[1]+dd[2])/sqrt(3)*alpha;
}

template<typename T>
T abs(T a){
  return a<0?-a:a;
}

uint32_t absDiffRGB(uint32_t a,uint32_t b){
  uint32_t result = 0;
  result |= abs((a&0x0000ffu) - (b&0x0000ffu));
  result |= abs((a&0x00ff00u) - (b&0x00ff00u));
  result |= abs((a&0xff0000u) - (b&0xff0000u));
  return result;
}

uint32_t thresholdRGB(uint32_t a,uint8_t t=220){
  uint32_t result = 0;
  result |= ((a&0x0000ffu) > (t<< 0u))*0x0000ff;
  result |= ((a&0x00ff00u) > (t<< 8u))*0x00ff00;
  result |= ((a&0xff0000u) > (t<<16u))*0xff0000;
  return result;
}

uint32_t invertRGB(uint32_t a){
  return absDiffRGB(0x00ffffff,a);
}

uint32_t samenessRGB(uint32_t r,uint32_t a){
  return invertRGB(absDiffRGB(r,a));
}

uint32_t minRGB(uint32_t a,uint32_t b){
  uint32_t result = 0;
  result |= min(a&0x0000ffu,b&0x0000ffu);
  result |= min(a&0x00ff00u,b&0x00ff00u);
  result |= min(a&0xff0000u,b&0xff0000u);
  return result;
}

uint32_t whiteFilter(uint32_t a){
  return a==0xffffff?0xffffff:0;
}

uint32_t computeLogoness(std::vector<uint32_t>const&colors){
  uint32_t thres0 = whiteFilter(thresholdRGB(colors[0]));
  uint32_t thres1 = whiteFilter(thresholdRGB(colors[1]));
  uint32_t diff = absDiffRGB(thres0,thres1);
  uint32_t result = diff;//minRGB(diff,thres);
  //result = 0x00ffffff;
  for(size_t i=2;i<colors.size();++i)
    result = minRGB(result,samenessRGB(thres1,whiteFilter(thresholdRGB(colors[i]))));
  return result;
}

struct RunningWindow{
  RunningWindow(size_t nframes){
    frames.resize(nframes);
  }
  void addFrame(cv::Mat const&f){
    addInitFrameIfPossible(f);
    addFrameToCyclicQueueIfPossible(f);
  }
  cv::Mat const&get(size_t f)const{
    auto const frameToGet = (firstFrame+f)%frames.size();
    return frames.at(frameToGet);
  }
  void computeResult(){
    int32_t const width = result.cols;
    int32_t const height = result.rows;
    ivec2 coord;
    auto colors = std::vector<uint32_t>(frames.size());
    for(coord.y=0;coord.y<height;++coord.y)
      for(coord.x=0;coord.x<width;++coord.x){
        for(size_t f=0;f<frames.size();++f)
          colors.at(f) = readColor(get(f),coord);
        auto logoness = computeLogoness(colors);
        //auto logoness = whiteFilter(thresholdRGB(colors[0]));
        writeColor(result,coord,logoness);
      }
      //int erosion_type = 0;
      ////erosion_type = cv::MORPH_RECT   ;
      //erosion_type = cv::MORPH_CROSS  ;
      //int erosion_size = 1;
      ////erosion_type = cv::MORPH_ELLIPSE;
      //cv::Mat element = getStructuringElement( erosion_type,
      //                 cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
      //                 cv::Point( erosion_size, erosion_size ) );
      //cv::Mat oo;
      //erode(result,oo, element );
      //result = oo;
  }
  bool isCyclicQueueFull()const{
    return storedFrames == frames.size();
  }
  bool isCyclicQueueEmpty()const{
    return storedFrames == 0;
  }
  bool cyclicQueueContainAnyFrame()const{
    return storedFrames > 0;
  }
  void addInitFrameIfPossible(cv::Mat const&f){
    if(isCyclicQueueFull())return;
    createResultFrame(f);
    frames.at(storedFrames++) = f.clone();
  }
  void addFrameToCyclicQueueIfPossible(cv::Mat const&f){
    if(!isCyclicQueueFull())return;
    frames.at(firstFrame) = f.clone();
    firstFrame = (firstFrame+1) % frames.size();
  }
  void createResultFrame(cv::Mat const&f){
    if(cyclicQueueContainAnyFrame())return;
    result = f.clone();
  }
  cv::Mat result;
  std::vector<cv::Mat>frames;
  size_t firstFrame = 0;
  size_t storedFrames = 0;
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

void loadVideo(vars::Vars&vars){
  if(notChanged(vars,"",{"videoFileName"}))return;
  auto const&videoName = vars.getString("videoFileName");

  auto video = vars.add<Video>("video",videoName);
  auto window = *vars.get<std::shared_ptr<sdl2cpp::Window>>("window");

  if(video->width < window->getWidth() || video->height < window->getHeight())
    window->setSize(video->width,video->height);
}

void createFrame(vars::Vars&vars){
  if(notChanged(vars,"",{"video"}))return;

  auto video = vars.get<Video>("video");
  vars.reCreate<ge::gl::Texture>("frame",GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
}

std::string loadTxtFile(std::string const&f){
  std::ifstream t(f);
  std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
  return str;
}

void loadFragment(vars::Vars&vars){
  *vars.reCreate<std::string>("fragmentSource") = loadTxtFile(vars.getString("fragmentSourceFile"));
}

void saveFragment(vars::Vars&vars){
  auto fn = vars.getString("fragmentSourceFile");
  auto f = std::ofstream(fn);
  auto data = vars.getString("framentSource");
  f.write(data.data(),data.size());
  f.close();
}

void EmptyProject::init(){
  *vars.add<std::shared_ptr<sdl2cpp::Window>>("window") = window;
  window->setEventCallback(SDL_MOUSEWHEEL,[&](SDL_Event const&e){mouseWheel(e);return true;});

  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto videoName  = args->gets("--video","","video to parse");
  auto startFrame = args->getu32("--start",0,"starting frame");
  auto nFrames    = args->getu32("--nFrames",100,"number of frames to process");
  auto printHelp  = args->isPresent("--help");
  if(printHelp||!args->validate()){
    std::cerr <<args->toStr() << std::endl;
  }

  vars.addString("videoFileName",videoName);
  loadVideo(vars);
  createFrame(vars);

  vars.add<ge::gl::VertexArray>("emptyVao");
  vars.addUVec2("windowSize",window->getWidth(),window->getHeight());
  vars.addMap<SDL_Keycode, bool>("input.keyDown");

  vars.addBool("compile");

  vars.addString("fragmentSourceFile","../drawVideo.fp");
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
  if(notChanged(vars,"",{"fragmentSource"}))return;

  std::cerr << "createProgram" << std::endl;
  std::string const vsSrc = R".(
  #version 450

  layout(binding=0)uniform usampler2D tex;
  uniform uvec2 windowSize = uvec2(512);
  uniform float zoom       = 1.f;
  uniform vec2 offset      = vec2(0);

  out vec2 vCoord;

  void main(){
    ivec2 frameSize = textureSize(tex,0).xy;

    vec2 scale = zoom*vec2(frameSize) / vec2(windowSize);

    vCoord = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4((-1+2*vCoord)*scale+offset,0,1);
  }

  ).";

  auto fsSrc = vars.getString("fragmentSource");
  auto prg = vars.reCreate<ge::gl::Program>(
      "program",
      std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER  ,vsSrc),
      std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,fsSrc)
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
      case GL_FLOAT       :
        {
          float v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),&v);
          vars.addFloat ("program."+name,v);break;
        }
      case GL_FLOAT_VEC2:
        {
          glm::vec2 v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),(float*)&v);
          vars.add<glm::vec2>("program."+name,v);break;
        }
      default:break;


    }


  }
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
  auto&src = vars.getString("fragmentSource");
  static char text[1024*100];
  std::strcpy(text,src.data());

  static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
  ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 32), flags);
  src = text;

  ImGui::Columns(3,"col",false);
  if(ImGui::Button("comp")){
    vars.updateTicks("fragmentSource");
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
      ImGui::EndMenu();
    }
  }
}

void EmptyProject::draw(){
  //watchFiles(vars);
  createProgram(vars);

  ge::gl::glClearColor(0.1f,0.1f,0.1f,1.f);
  ge::gl::glClear(GL_COLOR_BUFFER_BIT);

  auto vao = vars.get<ge::gl::VertexArray>("emptyVao");
  auto prg = vars.get<ge::gl::Program>("program");
  auto tex = vars.get<ge::gl::Texture>("frame");
  auto video = vars.get<Video>("video");

  if(vars.addOrGetBool("playVideo",true)){
    video->readFrame();
    tex->setData2D(video->frame.data,GL_BGR_INTEGER);
  }
  vars.addOrGetUint32("frameId") = video->getFrameId();
  vars.addOrGetString("time") = getTimeFormat(video->getFrameId(),video->fps);

  prg->set2uiv("windowSize",(uint32_t*)&(vars.getUVec2("windowSize")));
 

  std::vector<std::string>uniformNames;
  vars.getDir(uniformNames,"program");
  for(auto const&n:uniformNames){
    auto vName = "program."+n;
    auto const&type = vars.getType(vName);
    if(type == typeid(uint32_t))prg->set1ui(n,vars.getUint32(vName));
    if(type == typeid( int32_t))prg->set1i (n,vars.getInt32 (vName));
    if(type == typeid(float   ))prg->set1f (n,vars.getFloat (vName));
    if(type == typeid(glm::vec2))prg->set2fv(n,(float*)&vars.getVec2(vName));
  }

  vao->bind();
  tex->bind(0);

  prg->use();

  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);

  vao->unbind();

  editProgram(vars);

  drawImguiVars(vars);

  swap();
}

void EmptyProject::key(SDL_Event const& event, bool DOWN) {
  auto&keys = vars.getMap<SDL_Keycode, bool>("input.keyDown");
  keys[event.key.keysym.sym] = DOWN;
  if(DOWN && event.key.keysym.sym == SDLK_SPACE)
    vars.getBool("playVideo") ^= true;
}

void EmptyProject::mouseWheel(SDL_Event const& event){
  auto&zoom = vars.getFloat("program.zoom");
  auto zoomInc = 0.1f*event.wheel.y;
  int x,y;
  SDL_GetMouseState(&x,&y);
  float mx = 2.f*(      (float)x / (float)window->getWidth ()) - 1.f;
  float my = 2.f*(1.f - (float)y / (float)window->getHeight()) - 1.f;

  glm::vec2 mouse = glm::vec2(mx,my);
  auto&offset = vars.getVec2("program.offset");
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
    auto&offset = vars.getVec2("program.offset");
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


/*
int main(int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto videoName  = args->gets("--video","","video to parse");
  auto startFrame = args->getu32("--start",0,"starting frame");
  auto nFrames    = args->getu32("--nFrames",100,"number of frames to process");
  auto printHelp  = args->isPresent("--help");
  if(printHelp||!args->validate()){
    std::cerr <<args->toStr() << std::endl;
    return 0;
  }

  auto video = Video(videoName);

  std::cerr << video.width*video.height*video.nofFrames*3 / 1024.f/1024.f << " MB" << std::endl;
  
  SDL_Init(SDL_INIT_EVERYTHING);
  auto win = SDL_CreateWindow("ogl",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,video.width,video.height,SDL_WINDOW_OPENGL);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
  auto context = SDL_GL_CreateContext(win);
  ge::gl::init();
  ge::gl::setHighDebugMessage();

  std::string const vsSrc = R".(
  #version 450

  void main(){
    gl_Position = vec4(-1+2*(gl_VertexID&1),-1+2*(gl_VertexID>>1),0,1);
  }

  ).";
  std::string const fsSrc = R".(
  #version 450

  layout(binding=0)uniform usampler2D tex;
  out vec3 fColor;

  uvec3 threshold(uvec3 c,uint t){
    return uvec3(c.x>t,c.y>t,c.z>t)*255;
  }
  uvec3 filterNonWhite(uvec3 c){
    return uvec3(all(equal(c,uvec3(255))))*255;
  }

  void main(){

    ivec2 size = textureSize(tex,0).xy;
    ivec2 coord = ivec2(gl_FragCoord.x,size.y-int(gl_FragCoord.y)-1);

    uvec3 color = texelFetch(tex,coord,0).rgb;
    color = filterNonWhite(threshold(color,230));

    fColor = vec3(color) / vec3(255);
    
  }
  ).";

  GLuint tt;
  ge::gl::glCreateTextures(GL_TEXTURE_2D,1,&tt);
  ge::gl::glTextureStorage2D(tt,1,GL_RGB8UI,video.width,video.height);
  ge::gl::glTextureParameteri(tt,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  ge::gl::glTextureParameteri(tt,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  auto vao = std::make_shared<ge::gl::VertexArray>();
  auto prg = std::make_shared<ge::gl::Program>(
      std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER,vsSrc),
      std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,fsSrc));

  video.moveToFrame(startFrame);
  auto endFrame = startFrame + nFrames;

  std::vector<uint8_t>data(video.width*video.height*3);
  for(auto&x:data)x=255;
  ge::gl::glTextureSubImage2D(tt,0,0,0,video.width,video.height,GL_RGB_INTEGER,GL_UNSIGNED_BYTE,data.data());//video.frame.data);

  bool stopVideo = false;

  bool running = true;
  SDL_Event event;
  while(running){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT)
        running = false;
      if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_q)
          running = false;
        if(event.key.keysym.sym == SDLK_SPACE)
          stopVideo = !stopVideo;
      }

    }

    if(!stopVideo){
      float videoLengthInSec = (float)video.nofFrames / (float)video.fps;
      float frameTime = (float)video.getFrameId() / (float)video.fps;
      float percent = (float)video.getFrameId() / (float)video.nofFrames * 100;
      std::cerr << "time: " << frameTime << " / " << videoLengthInSec << " - " << percent << "%" << std::endl;
      video.readFrame();
      ge::gl::glPixelStorei(GL_UNPACK_ROW_LENGTH,video.frame.cols);
      ge::gl::glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      ge::gl::glTextureSubImage2D(tt,0,0,0,video.width,video.height,GL_BGR_INTEGER,GL_UNSIGNED_BYTE,video.frame.data);
    }

    ge::gl::glClearColor(0,0.1,0,1);
    ge::gl::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    ge::gl::glBindTextureUnit(0,tt);
    vao->bind();
    prg->use();
    ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    vao->unbind();

    SDL_GL_SwapWindow(win);

    if(video.getFrameId() >= video.nofFrames)running = false;
  }
  ge::gl::glDeleteTextures(1,&tt);


  auto out = video.frame.clone();
  auto old = video.frame.clone();

  size_t nwindow = 10;
  auto window = RunningWindow(nwindow);
  for(size_t i=0;i<nwindow;++i){
    video.readFrame();
    window.addFrame(video.frame);
  }

  for(size_t i=0;i<endFrame;++i){
    window.computeResult();

    std::stringstream ss;
    ss << "frame" << std::setfill('0') << std::setw(5) << i << ".png";
    auto name = ss.str();
    std::cerr << name << std::endl;
    imwrite(name.c_str(),window.result);

    video.readFrame();
    window.addFrame(video.frame);
  }
  return 0;


  video.readFrame();
  old = video.frame.clone();
  //cv::threshold(video.frame,old,230,255,cv::THRESH_BINARY);

  for(size_t i=0;i<1000;++i){
    std::stringstream ss;
    ss << "frame" << std::setfill('0') << std::setw(5) << i << ".png";
    auto name = ss.str();
    std::cerr << name << std::endl;
    video.readFrame();
    cv::absdiff(old,video.frame,out);
    //cv::threshold(video.frame,out,230,255,cv::THRESH_BINARY);


    imwrite(name.c_str(),out);
    old = video.frame.clone();
  }
  return 0;

  auto normalFont = Font("../normal.png");
  //for(auto const&l:normalFont.lts)
  //  l.save();

  auto conv = video.frame.clone();
  auto const&l = normalFont.get('a');

  auto iw = video.frame.cols;
  auto ih = video.frame.rows;
  auto lw = l.img.cols;
  auto lh = l.img.rows;

  float letterOcc = 0.f;
  for(int32_t y=0;y<lh;++y)
    for(int32_t x=0;x<lw;++x)
      letterOcc += l.img.data[(y*l.img.cols+x)*4+3]/255.f;

  std::cerr << "letterOcc: " << letterOcc << std::endl;
  std::cerr << "i: " << iw << " " << ih << std::endl;
  std::cerr << "l: " << lw << " " << lh << std::endl;
  std::cerr << "c: " << conv.cols << " " << conv.rows << std::endl;

  for(int32_t y=0;y<ih-lh;++y)
    for(int32_t x=0;x<iw-lw;++x){
      float distance = 0;
      for(int32_t j=0;j<lh;++j)
        for(int32_t i=0;i<lw;++i){
          auto frameColor = readColor(video.frame,ivec2(x+i,y+j));
          auto letterColor = readColor(l.img,ivec2(i,j));
          distance += colorDistance(frameColor,letterColor);

        }
      distance /= letterOcc;//(float)(lw * lh);
      for(int32_t c=0;c<conv.channels();++c)
        conv.data[(y*conv.cols+x)*conv.channels()+c] = (uint8_t)(sqrt(distance) * 255);
    }


  imwrite("distance.png",conv);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
*/
