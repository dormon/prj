#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <SDL2CPP/MainLoop.h>
#include <SDL2CPP/Window.h>
#include <imguiSDL2OpenGL/imgui.h>
#include <sstream>

using namespace ge::gl;
using namespace std;

template<typename RETURN,typename...ARGS>
class Barrier{
  public:
    using PTR = RETURN(*)(ARGS...);
    Barrier(PTR const&,RETURN && defRet,ARGS && ... defaults):returnValue(defRet),arguments{defaults...}{}
    bool notChanged(ARGS const&... args){
      auto newInputs = std::tuple<ARGS...>(args...);
      auto same = arguments == newInputs;
      if(same)return same;
      arguments = newInputs;
      return same;
    }
    RETURN             returnValue;
    std::tuple<ARGS...>arguments  ;
};

template<typename RETURN,typename...ARGS,typename VRET,typename...VARGS>
Barrier<RETURN,ARGS...>make_Barrier(RETURN(*ptr)(ARGS...),VRET && returnDef,VARGS && ...defaults){
  return Barrier<RETURN,ARGS...>{ptr,static_cast<RETURN>(returnDef),static_cast<ARGS>(defaults)...};
}


shared_ptr<Program> getProgram(){
  stringstream vs;
  vs << "#version      " << 450      << endl;
  vs << "#line         " << __LINE__ << endl;
  vs << R".(
  out vec2 vCoord;
  void main(){
    vec2 c = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4(-1+2*c,0,1);
    vCoord = c;
  }
  ).";
  stringstream fs;
  fs << "#version      " << 450      << endl;
  fs << "#line         " << __LINE__ << endl;
  fs << R".(

  uint rand(uint x){
    return x*(x*(x*(x*(x+133321)+18849)+1377330)+132390);
  }

  float randf(uint x){
    return float(rand(x)) / float(0xffffffffu);
  }

  uint rrand(uint x){
    x = (x>>16) ^ (x&0xffffu);
    x = (x>>8) ^ (x&0xffu);
    x = (x>>4) ^ (x&0xfu);
    x = (x>>2) ^ (x&0x5u);
    x = (x>>1) ^ (x&0x1u);
  }

  in vec2 vCoord;
  out vec4 fColor;
  void main(){
    if(randf(uint(gl_FragCoord.x)) < vCoord.y)
      fColor = vec4(0,.4,0,1);
    else
      fColor = vec4(0,0,.5,1);
    //fColor = vec4(randf(uint(gl_FragCoord.x)));
  }
  ).";
  return make_shared<Program>(make_shared<Shader>(GL_VERTEX_SHADER,vs.str()),make_shared<Shader>(GL_FRAGMENT_SHADER,fs.str()));
}

int main(int argc,char*argv[]){
  auto mainLoop = make_shared<sdl2cpp::MainLoop>();
  auto window   = make_shared<sdl2cpp::Window  >();
  window->createContext("rendering");
  ge::gl::init();
  mainLoop->addWindow("mainWindow",window);
  auto imgui = std::make_unique<imguiSDL2OpenGL::Imgui>(window->getWindow());
  mainLoop->setEventHandler([&](SDL_Event const&event){
    return imgui->processEvent(&event);
  });
  auto qr = make_shared<AsynchronousQuery>(GL_TIME_ELAPSED,GL_QUERY_RESULT,AsynchronousQuery::UINT64);
  auto vao = make_shared<VertexArray>();
  auto program = getProgram();

  double time = 0.;
  double bandwidthInGigabytes = 0.;
  bool show_demo_window = true;
  mainLoop->setIdleCallback([&]{
    glClearColor(0.f,0.1f,0.1f,1.f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    imgui->newFrame(window->getWindow());

    vao->bind();
    program->use();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    vao->unbind();

    ImGui::Begin("vars");
    /*
    ImGui::Checkbox("use read program",&useReadProgram);
    ImGui::DragScalar("floatsPerThread"     ,ImGuiDataType_U64,&floatsPerThread   ,1,&minFloatsPerThread   ,&maxFloatsPerThread   );
    ImGui::DragScalar("workGroupSize"       ,ImGuiDataType_U64,&workGroupSize     ,1,&minWorkGroupSize     ,&maxWorkGroupSize     );
    ImGui::DragScalar("number of workgroups",ImGuiDataType_U64,&nofWorkGroups     ,1,&minNofWorkGroups     ,&maxNofWorkGroups     );
    if(useReadProgram)
      ImGui::DragScalar("registers per thread",ImGuiDataType_U64,&registersPerThread,1,&minRegistersPerThread,&maxRegistersPerThread);
    ImGui::Text("buffer Size : %f [GB]"   ,static_cast<float>(workGroupSize * nofWorkGroups * floatsPerThread * sizeof(float))/static_cast<float>(gigabyte));
    ImGui::Text("time        : %lf [s]"   ,time);
    ImGui::Text("bandwidth   : %lf [GB/s]",bandwidthInGigabytes);*/
    ImGui::End();
    imgui->render(window->getWindow(), window->getContext("rendering"));
    window->swap();
  });
  (*mainLoop)();
  imgui = nullptr;
  return 0;
}

