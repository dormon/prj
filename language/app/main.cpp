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

struct SwindowParam{
  unsigned Size[2];
  bool     Fullscreen;
}windowParam;

struct SCameraParam{
  float Near;
  float Far;
  float Fovy;
}cameraParam;

bool DisableAnttweakbar=true;
ge::util::ArgumentObject*Args;
ge::util::WindowObject*window;
ge::gl::VertexArrayObject*EmptyVAO;
ge::gl::ProgramObject*drawProgram=NULL;
lang::Generator*gen;
coll::Collision col;

ge::util::CameraObject*camera;
glm::vec3 Pos=glm::vec3(0.f,10.f,0.f);
float angle[3]={0,0,0};
float Speed=.01f;

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 mvp;


int main(int Argc,char*Argv[]){
  Args=new ge::util::ArgumentObject(Argc,Argv);

  ShaderDir=Args->getArg("--shader-directory","");

  DisableAnttweakbar=Args->isPresent("--disable-anttweakbar");

  //window args
  windowParam.Size[0] =atoi(Args->getArg("-w","1024").c_str());
  windowParam.Size[1]=atoi(Args->getArg("-h","1024").c_str());
  windowParam.Fullscreen=Args->isPresent("-f");

  //context args
  ContextParam.Version=atoi(Args->getArg("--context-version","430").c_str());
  ContextParam.Profile=Args->getArg("--context-profile","compatibility");
  ContextParam.Flag=Args->getArg("--context-flag","debug");

  //camera args
  cameraParam.Near = ge::util::atof(Args->getArg("-near","0.1").c_str());
  cameraParam.Far  = ge::util::atof(Args->getArg("-far" ,"inf").c_str());
  cameraParam.Fovy = ge::util::atof(Args->getArg("-fovy","45" ).c_str());


  window=new ge::util::WindowObject(
      windowParam.Size[0],
      windowParam.Size[1],
      windowParam.Fullscreen,
      Idle,
      Mouse,
      !DisableAnttweakbar,
      ContextParam.Version,
      ContextParam.Profile,
      ContextParam.Flag);

  glewExperimental=GL_TRUE;
  glewInit();
  ge::gl::initShadersAndPrograms();
  ge::gl::setDefaultDebugMessage();


  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  Init();
  window->mainLoop();
  delete window;
  delete Args;
  return 0;
}

void Mouse(){
  if(window->isLeftDown()){
    angle[1]+=window->getDeltaMousePosition()[0]*.01;
    angle[0]+=window->getDeltaMousePosition()[1]*.01;
  }
  if(window->isMiddleDown()){
    angle[2]+=window->getDeltaMousePosition()[0]*.01;
  }
}

void Wheel(int d){
}


class TriangleSymbol: public lang::Symbol
{
  public:
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;
    TriangleSymbol(glm::vec2 a,glm::vec2 b,glm::vec2 c):lang::Symbol("triangle2"){
      this->a=a;
      this->b=b;
      this->c=c;
    }
    void draw(){
      glBegin(GL_TRIANGLES);
      glVertex4f(a.x,a.y,0,1);
      glVertex4f(b.x,b.y,0,1);
      glVertex4f(c.x,c.y,0,1);
      glEnd();
    }
};





class FractTriangle: public lang::Rule
{
  public:
    FractTriangle():lang::Rule("generator","triangle2"){}
    lang::String*body(lang::Generator*gen){
      lang::String*result=new lang::String();
      TriangleSymbol*parent=(TriangleSymbol*)gen->currentString->string[gen->currentPosition];
      glm::vec2 ab=(parent->a+parent->b)/2.f;
      glm::vec2 ac=(parent->a+parent->c)/2.f;
      glm::vec2 bc=(parent->b+parent->c)/2.f;
      result->string.push_back(new TriangleSymbol(parent->a,ab,ac));
      result->string.push_back(new TriangleSymbol(parent->b,bc,ab));
      result->string.push_back(new TriangleSymbol(parent->c,ac,bc));
      return result;
    }
};

void Init(){
  camera=new ge::util::CameraObject(
      windowParam.Size,
      cameraParam.Near,
      cameraParam.Far,
      cameraParam.Fovy);

  /*
  lang::String*startString=new lang::String();
  startString->string.push_back(new TriangleSymbol(glm::vec2(0.f,0.f),glm::vec2(1.f,0.f),glm::vec2(0.5f,1.f)));
  gen=new lang::Generator(1000,startString);
  gen->rules.push_back(new FractTriangle());
  std::cerr<<gen->rules[0]->getString()<<std::endl;
  std::cerr<<gen->getCurrentString()<<std::endl;
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  // */

  /*
  lang::String*startString=new lang::String();
  startString->string.push_back(
      new QuadSymbol(glm::vec2(0.f,0.f),glm::vec2(1.f,0.f),glm::vec2(0.f,1.f)));
  gen=new lang::Generator(1000,startString);
  gen->rules.push_back(new RandomQuadSplit(.3,.8));
  gen->rules.push_back(new QuadTo4Quad(.1));
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  // */
  /*
  lang::String*startString=new lang::String();
  startString->string.push_back(
      new lang::SystemSymbol(
        glm::vec3(0.f,0.f,0.f),
        glm::vec3(1.f,0.f,0.f),
        glm::vec3(0.f,1.f,0.f),
        glm::vec3(0.f,0.f,.04f),
        "systemA"));
  gen=new lang::Generator(1000,startString);
  gen->rules.push_back(new SystemToBox(glm::vec3(.9f),"systemB"));
  gen->rules.push_back(new lang::GridXY(glm::vec2(0.f),glm::uvec2(4u,4u),"systemA","systemB"));
  gen->next();
  gen->next();




  // */
  //*
  lang::String*startString=new lang::String();
  startString->string.push_back(
      new lang::SystemSymbol(
        glm::vec3(1.f,0.f,0.f),
        glm::vec3(1.f,0.f,0.f),
        glm::vec3(0.f,1.f,0.f),
        glm::vec3(0.f,0.f,1.f),
        "systemA"));
  gen=new lang::Generator(1000,startString);
  //gen->rules.push_back(new lang::GridXY(glm::vec2(0.f),glm::uvec2(4u,4u),"systemA","systemB"));
  //gen->rules.push_back(new lang::GridXY(glm::vec2(0.f),glm::uvec2(4u,4u),"systemB","systemB"));
  //"systemA systemB gridXY(vec2(0),uvec2(4u)"
  gen->rules.push_back(new lang::GridXY(glm::vec2(0.f),glm::uvec2(4u),"systemA","systemB"));
  gen->rules.push_back(new lang::CircleXY(.1,10,0,glm::pi<float>()*2,"systemB","systemC"));
  gen->rules.push_back(new lang::CircleXY(.1,20,0,glm::pi<float>()*2,"systemC","systemD"));

  gen->rules.push_back(new SystemToBox(glm::vec3(1.f),"systemD"));



  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
//  gen->next();
//  gen->next();






  // */

/*  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();
  gen->next();*/
  //std::cerr<<gen->currentString->getString()<<std::endl;

  lang::String*str=gen->currentString;
  for(unsigned t=0;t<str->string.size();++t){
    if(str->string[t]->getName()=="box")
      col.boxes.push_back((BoxSymbol*)str->string[t]);
  }


  drawProgram = new ge::gl::ProgramObject(
      ShaderDir+"draw.vp",
      ShaderDir+"draw.fp");


  EmptyVAO=new ge::gl::VertexArrayObject();
  boxInit(ShaderDir);
  //std::cerr<<gen->getCurrentString()<<std::endl;

}

int whichIteration=0;
bool yDown=false;
bool xDown=false;

void Idle(){
  camera->fpsCamera(angle[0],angle[1],angle[2]);
  glm::vec3 oldPosition=-camera->getPosition();
  camera->right  ((window->isKeyDown('d')-window->isKeyDown('a'))*Speed);
  camera->up     ((window->isKeyDown(' ')-window->isKeyDown('c'))*Speed);
  camera->forward((window->isKeyDown('w')-window->isKeyDown('s'))*Speed);
  glm::vec3 newPosition=-camera->getPosition();
  glm::vec3 normal;
  if(col.collision(normal,oldPosition,newPosition,.2)<1.0f&&false){
    glm::vec3 v=newPosition-oldPosition;
    v-=normal*glm::dot(normal,v);
    //std::cerr<<normal.x<<" "<<normal.y<<" "<<normal.z<<std::endl;a
    camera->right  ((-window->isKeyDown('d')+window->isKeyDown('a'))*Speed);
    camera->up     ((-window->isKeyDown(' ')+window->isKeyDown('c'))*Speed);
    camera->forward((-window->isKeyDown('w')+window->isKeyDown('s'))*Speed);
    camera->setPosition(camera->getPosition()-v);
  }



  Pos=camera->getPosition();

  camera->getView(&View);
  camera->getProjection(&Projection);
  mvp=Projection*View;


  if(window->isKeyDown('y')){
    if(!yDown){
      whichIteration++;
      if(whichIteration>=(int)gen->strings.size())whichIteration=0;
    }
    yDown=true;
  }else yDown=false;
  if(window->isKeyDown('x')){
    if(!xDown){
      whichIteration--;
      if(whichIteration<0)whichIteration=gen->strings.size()-1;
    }
    xDown=true;
  }else xDown=false;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  drawProgram->use();
  drawProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));

  glColor4f(1,1,1,1);
  lang::String*str=gen->strings[whichIteration];
  for(unsigned t=0;t<str->string.size();++t){
    lang::Symbol*s=str->string[t];
    //if(t==0)std::cerr<<s->getName()<<std::endl;
    s->draw(mvp);
  }
  window->swap();
}

