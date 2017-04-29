#include"main.hpp"

#include<geCore/TypeRegister.h>
#include<geCore/Accessor.h>
#include<geCore/functionRegister.h>
#include<geCore/function.h>
#include<geCore/stdFunctions.h>
#include<geCore/macroFunction.h>
#include<geCore/text.h>



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
  auto typeRegister = std::make_shared<ge::core::TypeRegister>();
  auto nameRegister = std::make_shared<ge::core::Namer>();
  auto functionRegister = std::make_shared<ge::core::FunctionRegister>(typeRegister,nameRegister);
  ge::core::registerStdFunctions(functionRegister);

  ge::core::registerBasicFunction(functionRegister,"loadTextFile",ge::core::loadTextFile);
  auto loadFile=functionRegister->sharedFunction("loadTextFile");
  auto fileName=functionRegister->sharedFunction("Nullary");
  fileName->bindOutput(typeRegister->sharedAccessor("string"));
  (string&)(*fileName->getOutputData()) = "app/main.cpp";
  loadFile->bindInput(0,fileName);
  loadFile->bindOutput(typeRegister->sharedAccessor("string"));

  (*loadFile)();
  std::cout<<(string&)(*loadFile->getOutputData())<<std::endl;

  // */

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

  //auto pof = std::make_shared<ProgramObjectFactory>(functionRegister);
  //(*pof)();
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

  Window->swap();
}

void Init(){
  glewExperimental=GL_TRUE;
  glewInit();

  ge::gl::initShadersAndPrograms();
  ge::gl::setDefaultDebugMessage();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
}


