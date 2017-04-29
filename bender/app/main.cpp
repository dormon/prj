#include"main.hpp"

void Init();
void Idle();
void Mouse();
void Wheel(int d);

ge::util::ContextParam contextParam;
ge::util::WindowParam  windowParam;

struct SCameraParam{
  float Near;
  float Far;
  float Fovy;
}CameraParam;

bool DisableAnttweakbar=true;
ge::util::ArgumentObject*Args;
ge::util::WindowObject*Window;
ge::gl::VertexArrayObject*EmptyVAO;


float Speed=1.f;
glm::vec3 Pos=glm::vec3(0.f,10.f,0.f);
float Angle[3]={0,0,0};

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 mvp;

bool Render=true;
bool Stop=false;

ge::util::CameraObject*Camera;
ge::gl::ProgramObject*MinecraftProgram;
ge::util::FPSPrinter*fpsPrinter;

int main(int Argc,char*Argv[]){
  Args=new ge::util::ArgumentObject(Argc,Argv);

  ShaderDir=Args->getArg("--shader-directory","");

  DisableAnttweakbar=Args->isPresent("--disable-anttweakbar");

  ge::util::loadWindowParam(&windowParam,Args);
  ge::util::loadContextParam(&contextParam,Args);

  //camera args
  CameraParam.Near = Args->getArgf("-near","0.1");
  CameraParam.Far  = Args->getArgf("-far" ,"inf");
  CameraParam.Fovy = Args->getArgf("-fovy","90" );

  Window=new ge::util::WindowObject(
      windowParam.size[0],
      windowParam.size[1],
      windowParam.fullscreen,
      Idle,
      Mouse,
      !DisableAnttweakbar,
      contextParam.version,
      contextParam.profile,
      contextParam.flag);

  glewExperimental=GL_TRUE;
  glewInit();
  ge::gl::initShadersAndPrograms();


  EmptyVAO=new ge::gl::VertexArrayObject();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  Init();
  Window->mainLoop();
  delete Window;
  delete Args;
  return 0;
}

void Mouse(){
  if(Window->isLeftDown()){
    Stop=false;

    Angle[1]+=Window->getDeltaMousePosition()[0]*.041;
    Angle[0]+=Window->getDeltaMousePosition()[1]*.041;
  }
  if(Window->isMiddleDown()){
    Angle[2]+=Window->getDeltaMousePosition()[0]*.041;
  }
  if(Window->isMiddleDown()){
    Render=true;
  }
}

void Wheel(int d){
}

float Time=0;


void Idle(){
  if(Window->isKeyDown('p'))Stop=false;
  if(Stop)return;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  Camera->fpsCamera(Angle[0],Angle[1],Angle[2]);
  Camera->right((Window->isKeyDown('d')-Window->isKeyDown('a'))*Speed);
  Camera->up((Window->isKeyDown(' ')-Window->isKeyDown('c'))*Speed);
  Camera->forward((Window->isKeyDown('w')-Window->isKeyDown('s'))*Speed);
  Pos=Camera->getPosition();

  Camera->getView(&View);
  Camera->getProjection(&Projection);
  mvp=Projection*View;

  if(Window->isKeyDown('r')){
    Render=true;
    Stop=true;
  }
  unsigned Depth=50;
  if(Render)Depth=50;

  MinecraftProgram->use();
  /*
  MinecraftProgram->Set("ProjectionMatrix",1,GL_FALSE,glm::value_ptr(Projection));
  MinecraftProgram->Set("ViewMatrix",1,GL_FALSE,glm::value_ptr(View));
  */
  glm::mat4 ViewRotation;
  Camera->getViewRoration(&ViewRotation);
  glm::mat4 InverseProjectionView=glm::inverse(Projection*ViewRotation);
  MinecraftProgram->set("InverseProjectionViewMatrix",1,GL_FALSE,
      glm::value_ptr(InverseProjectionView));

  MinecraftProgram->set("Time",Time);
  MinecraftProgram->set("WindowSize",1,windowParam.size);
  MinecraftProgram->set("Depth",Depth);
  MinecraftProgram->set("CameraPos",-Pos[0],-Pos[1],-Pos[2]);
  MinecraftProgram->set("mode",(int)Render);
  Time+=.1;
  


  /*glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);*/

  EmptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  EmptyVAO->unbind();

  Window->swap();
  Render=false;
  fpsPrinter->endOfFrame();
}

void Init(){

  MinecraftProgram=new ge::gl::ProgramObject(
      ShaderDir+"minecraft.vp",
      ShaderDir+"minecraft.gp",
      ShaderDir+"minecraft.fp");

  Camera=new ge::util::CameraObject(
      windowParam.size,
      CameraParam.Near,
      CameraParam.Far,
      CameraParam.Fovy);

  fpsPrinter = new ge::util::FPSPrinter(2000);
  fpsPrinter->start();
}


