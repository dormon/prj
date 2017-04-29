#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<fstream>
#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<geGL/geGL.h>
#include"shaders.h"
#include"loadTextFile.h"
#include"window.h"

std::shared_ptr<EventProc>mainLoop = nullptr;

void draw(){
  mainLoop->makeCurrent("mainWindow");
  glClear(GL_COLOR_BUFFER_BIT);
  mainLoop->swap("mainWindow");
}

static const std::vector<GLenum>programInterface={
  GL_ATOMIC_COUNTER_BUFFER             ,
  GL_BUFFER_VARIABLE                   ,
  GL_UNIFORM                           ,
  GL_UNIFORM_BLOCK                     ,
  GL_PROGRAM_INPUT                     ,
  GL_PROGRAM_OUTPUT                    ,
  GL_SHADER_STORAGE_BLOCK              ,
  GL_VERTEX_SUBROUTINE                 ,
  GL_TESS_CONTROL_SUBROUTINE           ,
  GL_TESS_EVALUATION_SUBROUTINE        ,
  GL_GEOMETRY_SUBROUTINE               ,
  GL_FRAGMENT_SUBROUTINE               ,
  GL_COMPUTE_SUBROUTINE                ,
  GL_VERTEX_SUBROUTINE_UNIFORM         ,
  GL_TESS_CONTROL_SUBROUTINE_UNIFORM   ,
  GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
  GL_GEOMETRY_SUBROUTINE_UNIFORM       ,
  GL_FRAGMENT_SUBROUTINE_UNIFORM       ,
  GL_COMPUTE_SUBROUTINE_UNIFORM        ,
  GL_TRANSFORM_FEEDBACK_BUFFER         ,
  GL_TRANSFORM_FEEDBACK_VARYING        ,      
};



static const std::vector<std::string>programInterfaceNames={
  "GL_ATOMIC_COUNTER_BUFFER"             ,
  "GL_BUFFER_VARIABLE"                   ,
  "GL_UNIFORM"                           ,
  "GL_UNIFORM_BLOCK"                     ,
  "GL_PROGRAM_INPUT"                     ,
  "GL_PROGRAM_OUTPUT"                    ,
  "GL_SHADER_STORAGE_BLOCK"              ,
  "GL_VERTEX_SUBROUTINE"                 ,
  "GL_TESS_CONTROL_SUBROUTINE"           ,
  "GL_TESS_EVALUATION_SUBROUTINE"        ,
  "GL_GEOMETRY_SUBROUTINE"               ,
  "GL_FRAGMENT_SUBROUTINE"               ,
  "GL_COMPUTE_SUBROUTINE"                ,
  "GL_VERTEX_SUBROUTINE_UNIFORM"         ,
  "GL_TESS_CONTROL_SUBROUTINE_UNIFORM"   ,
  "GL_TESS_EVALUATION_SUBROUTINE_UNIFORM",
  "GL_GEOMETRY_SUBROUTINE_UNIFORM"       ,
  "GL_FRAGMENT_SUBROUTINE_UNIFORM"       ,
  "GL_COMPUTE_SUBROUTINE_UNIFORM"        ,
  "GL_TRANSFORM_FEEDBACK_BUFFER"         ,
  "GL_TRANSFORM_FEEDBACK_VARYING"        ,
};

static const std::vector<GLenum>programInterfacePname={
  GL_ACTIVE_RESOURCES              ,
  GL_MAX_NAME_LENGTH               ,
  GL_MAX_NUM_ACTIVE_VARIABLES      ,
  GL_MAX_NUM_COMPATIBLE_SUBROUTINES,
};

static const std::vector<std::string>programInterfacePnameNames={
  "GL_ACTIVE_RESOURCES"              ,
  "GL_MAX_NAME_LENGTH"               ,
  "GL_MAX_NUM_ACTIVE_VARIABLES"      ,
  "GL_MAX_NUM_COMPATIBLE_SUBROUTINES",
};

static const std::vector<GLenum>programInterfaceProperty={
  GL_ACTIVE_VARIABLES                    ,
  GL_BUFFER_BINDING                      ,
  GL_NUM_ACTIVE_VARIABLES                ,
  GL_ARRAY_SIZE                          ,
  GL_ARRAY_STRIDE                        ,
  GL_BLOCK_INDEX                         ,
  GL_IS_ROW_MAJOR                        ,
  GL_MATRIX_STRIDE                       ,
  GL_ATOMIC_COUNTER_BUFFER_INDEX         ,
  GL_BUFFER_DATA_SIZE                    ,
  GL_NUM_COMPATIBLE_SUBROUTINES          ,
  GL_COMPATIBLE_SUBROUTINES              ,
  GL_IS_PER_PATCH                        ,
  GL_LOCATION                            ,
  GL_LOCATION_COMPONENT                  ,
  GL_LOCATION_INDEX                      ,
  GL_NAME_LENGTH                         ,
  GL_OFFSET                              ,
  GL_REFERENCED_BY_VERTEX_SHADER         ,
  GL_REFERENCED_BY_TESS_CONTROL_SHADER   ,
  GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
  GL_REFERENCED_BY_GEOMETRY_SHADER       ,
  GL_REFERENCED_BY_FRAGMENT_SHADER       ,
  GL_REFERENCED_BY_COMPUTE_SHADER        ,
  GL_TRANSFORM_FEEDBACK_BUFFER_INDEX     ,
  GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE    ,
  GL_TOP_LEVEL_ARRAY_SIZE                ,
  GL_TOP_LEVEL_ARRAY_STRIDE              ,
  GL_TYPE                                ,
};

static const std::vector<std::string>programInterfacePropertyName={
  "GL_ACTIVE_VARIABLES"                    ,
  "GL_BUFFER_BINDING"                      ,
  "GL_NUM_ACTIVE_VARIABLES"                ,
  "GL_ARRAY_SIZE"                          ,
  "GL_ARRAY_STRIDE"                        ,
  "GL_BLOCK_INDEX"                         ,
  "GL_IS_ROW_MAJOR"                        ,
  "GL_MATRIX_STRIDE"                       ,
  "GL_ATOMIC_COUNTER_BUFFER_INDEX"         ,
  "GL_BUFFER_DATA_SIZE"                    ,
  "GL_NUM_COMPATIBLE_SUBROUTINES"          ,
  "GL_COMPATIBLE_SUBROUTINES"              ,
  "GL_IS_PER_PATCH"                        ,
  "GL_LOCATION"                            ,
  "GL_LOCATION_COMPONENT"                  ,
  "GL_LOCATION_INDEX"                      ,
  "GL_NAME_LENGTH"                         ,
  "GL_OFFSET"                              ,
  "GL_REFERENCED_BY_VERTEX_SHADER"         ,
  "GL_REFERENCED_BY_TESS_CONTROL_SHADER"   ,
  "GL_REFERENCED_BY_TESS_EVALUATION_SHADER",
  "GL_REFERENCED_BY_GEOMETRY_SHADER"       ,
  "GL_REFERENCED_BY_FRAGMENT_SHADER"       ,
  "GL_REFERENCED_BY_COMPUTE_SHADER"        ,
  "GL_TRANSFORM_FEEDBACK_BUFFER_INDEX"     ,
  "GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE"    ,
  "GL_TOP_LEVEL_ARRAY_SIZE"                ,
  "GL_TOP_LEVEL_ARRAY_STRIDE"              ,
  "GL_TYPE"                                ,
};

std::string programInterface2Str(GLenum interface){
  unsigned index=0;
  for(auto x:programInterface){
    if(x==interface)
      return programInterfaceNames[index];
    ++index;
  }
  return "unknown";
}

std::string programInterfacePname2Str(GLenum pname){
  unsigned index=0;
  for(auto x:programInterfacePname){
    if(x==pname)
      return programInterfacePnameNames[index];
    ++index;
  }
  return "unknown";
}

/*
class InterfaceData{
  public:
    GLuint      index         = 0 ;
    std::string name          = "";
    GLint       location      = -1;
    GLint       locationIndex = 0 ;
    InterfaceData(GLuint program, GLenum interface,){
      glGetProgramResourceName(program,interface,)
      for(GLint i=0;i<pname.pnames[GL_ACTIVE_RESOURCES];++i){

      }
    }
};*/

class InterfacePname{
  public:
    GLenum interface;
    std::map<GLenum,GLint>pnames;
//    std::vector<InterfaceData>data;
  public:
    InterfacePname(GLenum interface = 0){
      this->interface = interface;
    }
    friend std::ostream& operator<<(std::ostream& stream,InterfacePname p){
      for(auto x:p.pnames)
        stream<<"  "<<programInterfacePname2Str(x.first)<<": "<<x.second<<std::endl;
      return stream;
    }
    void getData(){




    }
};

class ProgramInterface{
  public:
    std::map<GLenum,InterfacePname>_interfaceParam;
    ProgramInterface(GLuint program){
      for(unsigned i=0;i<programInterface.size();++i){
        InterfacePname interfacePname(programInterface[i]);
        for(unsigned p=0;p<programInterfacePname.size();++p){
          GLint val=0;
          glGetProgramInterfaceiv(program,programInterface[i],programInterfacePname[p],&val);
          interfacePname.pnames[programInterfacePname[p]]=val;
        }
        this->_interfaceParam[programInterface[i]]=interfacePname;
      }
    }
    friend std::ostream& operator<<(std::ostream& stream,ProgramInterface i){
      for(auto x:i._interfaceParam){
        stream<<programInterface2Str(x.first)<<":"<<std::endl;
        stream<<x.second;
      }
      return stream;
    }
};

void init(){
  glewInit();
  ge::gl::setHighDebugMessage();
  glClearColor(0,1,0,1);
  GLuint program=createProgram(
      createShader(GL_VERTEX_SHADER  ,loadTextFile("shaders/simple.vp")),
      createShader(GL_FRAGMENT_SHADER,loadTextFile("shaders/simple.fp")));

  ProgramInterface pi(program);
  std::cout<<pi<<std::endl;
  glDeleteProgram(program);
}



int main(){
  mainLoop = std::make_shared<EventProc>();
  mainLoop->addWindow("mainWindow",std::make_shared<Window>(1024,768));
  mainLoop->getWindow("mainWindow")->createContext();
  mainLoop->setIdleCallback("mainWindow",draw);
  init();
  (*mainLoop)();
  mainLoop = nullptr;
  return 0;
}
