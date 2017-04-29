#include"main.hpp"

void Init();
void Idle();
void Mouse();
void Wheel(int d);

struct SContextParam{
  unsigned    Version;
  std::string Profile;
  std::string Flag;
}ContextParam;

struct SWindowParam{
  unsigned Size[2];
  bool     Fullscreen;
}WindowParam;

bool DisableAnttweakbar=true;
ge::util::ArgumentObject        *Args;
ge::util::WindowObject          *Window;
ge::gl::BufferObject            *readBuffer;
ge::gl::ProgramObject           *readProgram;
ge::gl::AsynchronousQueryObject *readQuery;

NDormon::CGpuPerfApi*GPA;


int main(int Argc,char*Argv[]){
  Args=new ge::util::ArgumentObject(Argc,Argv);

  ShaderDir=Args->getArg("--shader-directory","shaders/");

  DisableAnttweakbar=Args->isPresent("--disable-anttweakbar");

  //window args
  WindowParam.Size[0] =atoi(Args->getArg("-w","1024").c_str());
  WindowParam.Size[1]=atoi(Args->getArg("-h","768").c_str());
  WindowParam.Fullscreen=Args->isPresent("-f");

  //context args
  ContextParam.Version=atoi(Args->getArg("--context-version","430").c_str());
  ContextParam.Profile=Args->getArg("--context-profile","core");
  ContextParam.Flag=Args->getArg("--context-flag","debug");

  Window=new ge::util::WindowObject(
      WindowParam.Size[0],
      WindowParam.Size[1],
      WindowParam.Fullscreen,
      Idle,
      Mouse,
      !DisableAnttweakbar,
      ContextParam.Version,
      ContextParam.Profile,
      ContextParam.Flag);


  Init();
  Window->mainLoop();
  delete Window;
  delete Args;
  return 0;
}

void Mouse(){

}

void Wheel(int d){
}

#define READ_BINDING_READBUFFER 0
#define WORKGROUP_SIZE (64)
#define READBUFFER_SIZE (1024*1024)
#define NOF_CHANNELS 4

#define BYTES_PER_TYPE 4
#define NANOSECONDS_IN_SECOND 1000000000
#define GIGA (1024*1024*1024)

void Idle(){
  glClearColor(0,0.1,0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  readProgram->use();
  readBuffer->bindBase(GL_SHADER_STORAGE_BUFFER,READ_BINDING_READBUFFER);
  readProgram->set("offset",1u);
  //readProgram->set("skip",true);
  glFinish();

  readQuery->begin();
  //glDispatchCompute(READBUFFER_SIZE/WORKGROUP_SIZE,1,1);
  readQuery->end();

  glFinish();

  //readProgram->set("skip",false);
#define NUM 100
  //readProgram->set("skip",(bool)Window->isKeyOn('c'));
  glFinish();
  readProgram->set("dat",0.f,0.f,0.f,0.f);
  unsigned long long acc=0;
  if(Window->isKeyOn('n')){
    GPA->BeginSession();
    for(unsigned p=0;p<GPA->GetNumPasses();++p){
      GPA->BeginPass();
      GPA->BeginSample(0);

      glDispatchCompute(20,1,1);

      GPA->EndSample();
      GPA->EndPass();
    }
    GPA->EndSession();
    std::cout<<GPA->GetResults()<<std::endl;

  }else{
    for(unsigned i=0;i<NUM;++i){
      readQuery->begin();
      glDispatchCompute(20*40*4*2,1,1);
      readQuery->end();
      acc+=readQuery->getui64();
    }
  }
  double execTime=(double)acc/(double)1000000000/(double)NUM;
  std::cout<<execTime<<std::endl;
  std::cout<<2*4*40*64*20*2048/execTime<<" flops/s"<<std::endl;
  //std::cerr<<(double)readQuery->getui64()/(double)1000000000<<std::endl;

  Window->swap();
}

void Init(){
  glewExperimental=GL_TRUE;
  glewInit();

  GPA=new NDormon::CGpuPerfApi(Window->getContext());
  GPA->EnableComputeShader();
  //GPA->EnableGeneral();

  ge::gl::initShadersAndPrograms();
  ge::gl::setDefaultDebugMessage();

  readProgram = new ge::gl::ProgramObject(
      ShaderDir+"read.comp",
      ge::gl::ShaderObject::define("READ_BINDING_READBUFFER",(int)READ_BINDING_READBUFFER)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE"         ,(int)WORKGROUP_SIZE         )+
      ge::gl::ShaderObject::define("NOF_CHANNELS"           ,(int)NOF_CHANNELS           )+
      "");
  readProgram->setRetrievable();

  readBuffer = new ge::gl::BufferObject(
      READBUFFER_SIZE*NOF_CHANNELS*sizeof(float)*2*32);

  readBuffer->clear(GL_R32F,GL_RED,GL_FLOAT);

  /*     NULL,
         GL_SHADER_STORAGE_BUFFER,
         GL_STATIC_DRAW);*/

  readQuery=new ge::gl::AsynchronousQueryObject(
      GL_TIME_ELAPSED,
      GL_QUERY_RESULT,
      ge::gl::AsynchronousQueryObject::UINT64);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
}


