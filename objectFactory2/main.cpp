#include<iostream>
#include<stdarg.h>
#include<string>
#include<cstring>
#include<fstream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geAd/WindowObject/WindowObject.h>
#include<geGL/geGL.h>
#include"textFileLoader.h"
#include"fileExtensionExtractor.h"
#include"fileExtensionTranslator.h"
#include"shaderObjectFactory.h"
#include"callExtensionTranslator.h"
#include"producer.h"
#include"registerStdFunctions.h"

std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;

void registerGEGLObjects(std::shared_ptr<ge::core::TypeRegister>&tr){
  tr->addClassD<ge::gl::ShaderObject           >("Shader"           );
  tr->addClassD<ge::gl::ProgramObject          >("Program"          );
  tr->addClassD<ge::gl::TextureObject          >("Texture"          );
  tr->addClassD<ge::gl::SamplerObject          >("Sampler"          );
  tr->addClassD<ge::gl::RenderbufferObject     >("Renderbuffer"     );
  tr->addClassD<ge::gl::FramebufferObject      >("Framebuffer"      );
  tr->addClassD<ge::gl::ProgramPipelineObject  >("ProgramPipeline"  );
  tr->addClassD<ge::gl::AsynchronousQueryObject>("AsynchronousQuery");
  tr->addClassD<ge::gl::VertexArrayObject      >("VertexArray"      );
  tr->addClassD<ge::gl::BufferObject           >("BufferObject"     );
  tr->addClassCD<std::shared_ptr<ge::gl::ShaderObject           >>("SharedShader"           );
  tr->addClassCD<std::shared_ptr<ge::gl::ProgramObject          >>("SharedProgram"          );
  tr->addClassCD<std::shared_ptr<ge::gl::TextureObject          >>("SharedTexture"          );
  tr->addClassCD<std::shared_ptr<ge::gl::SamplerObject          >>("SharedSampler"          );
  tr->addClassCD<std::shared_ptr<ge::gl::RenderbufferObject     >>("SharedRenderbuffer"     );
  tr->addClassCD<std::shared_ptr<ge::gl::FramebufferObject      >>("SharedFramebuffer"      );
  tr->addClassCD<std::shared_ptr<ge::gl::ProgramPipelineObject  >>("SharedProgramPipeline"  );
  tr->addClassCD<std::shared_ptr<ge::gl::AsynchronousQueryObject>>("SharedAsynchronousQuery");
  tr->addClassCD<std::shared_ptr<ge::gl::VertexArrayObject      >>("SharedVertexArray"      );
  tr->addClassCD<std::shared_ptr<ge::gl::BufferObject           >>("SharedBufferObject"     );
}

class PO{
  protected:
    unsigned _id = 0;
    unsigned*_d  = nullptr;
  public:
    PO(unsigned i){this->_id=i;std::cout<<"constructor: "<<this->_id<<std::endl;this->_d=new unsigned[10];}
    ~PO(){std::cout<<"desctructor: "<<this->_id<<std::endl;delete[]this->_d;}
    unsigned getId(){return this->_id;}
};

std::shared_ptr<ge::util::WindowObject>window = nullptr;

void idle();


typedef std::shared_ptr<ge::core::Function>(*BoxFactory)(std::shared_ptr<ge::core::TypeRegister>const&);

std::shared_ptr<Producer>pro = nullptr;


int main(){
  typeRegister = std::make_shared<ge::core::TypeRegister>();
  registerGEGLObjects(typeRegister);
  typeRegister->addType("floatFce",ge::core::TypeRegister::FCE,"f32",1,"f32");
  typeRegister->addClassD<PO>("PO");
  typeRegister->addClassD<ge::gl::ProgramObject>("ProgramObject");
  typeRegister->addClassCD<std::shared_ptr<ge::gl::ShaderObject>>("SharedShader");
  typeRegister->addClassD<std::shared_ptr<ge::core::Function>>("Function");
  typeRegister->addType("string2u32",ge::core::TypeRegister::FCE,ge::core::TypeRegister::U32,1,ge::core::TypeRegister::STRING);
  pro = std::make_shared<Producer>(typeRegister);

  pro->registerFunction(ge::core::Nullary      ::name(),ge::core::Nullary      ::sharedInstance);
  pro->registerFunction(TextFileLoader         ::name(),TextFileLoader         ::sharedInstance);
  pro->registerFunction(FileExtensionExtractor ::name(),FileExtensionExtractor ::sharedInstance);
  pro->registerFunction(FileExtensionTranslator::name(),FileExtensionTranslator::sharedInstance);
  pro->registerFunction(CallExtensionTranslator::name(),CallExtensionTranslator::sharedInstance);
  pro->registerFunction(ShaderObjectFactory    ::name(),ShaderObjectFactory    ::sharedInstance);


  registerStdFunctions(pro);



  auto nsasfet = pro->fce("Nullary");
  auto sasfet  = pro->res("string2u32");
  auto fet     = pro->fce("FileExtensionTranslator");
  auto tfl     = pro->fce("TextFileLoader");
  auto cfn     = pro->fce("Nullary");
  auto sacfn   = pro->res("string");
  *sacfn = std::string("glslFileExtension.cfg");
  cfn->bindOutput(sacfn);
  tfl->bindInput("fileName",cfn);
  tfl->bindOutput(pro->res("string"));
  fet->bindInput("config",tfl);
  *sasfet = fet;
  nsasfet->bindOutput(sasfet);

  auto sid    = pro->res("u32");
  auto caller = pro->fce("CallExtensionTranslator");
  auto fee    = pro->fce("FileExtensionExtractor");
  auto sfn    = pro->fce("Nullary");
  auto sasfn  = pro->res("string");
  *sasfn = std::string("triangle.gp");
  sfn->bindOutput(sasfn);
  fee->bindInput("fileName",sfn);
  fee->bindOutput(pro->res("string"));
  caller->bindOutput(sid);
  caller->bindInput("fileExtensionTranslator",nsasfet);
  caller->bindInput("fileExtension",fee);
  (*caller)();

  auto sh = pro->fce("ShaderObjectFactory");
  auto fl = pro->fce("TextFileLoader");
  fl->bindOutput(pro->res("string"));
  fl->bindInput("fileName",sfn);
  sh->bindInput("source",fl);
  sh->bindInput("type",caller);
  sh->bindOutput(pro->res("SharedShader"));


  std::cerr<<(unsigned&)*caller->getOutput()<<std::endl;


/*
  auto ff=typeRegister->sharedAccessor<std::shared_ptr<ge::core::Function>>("Function");

  //auto a=typeRegister->sharedAccessor<PO>("PO",0);
  auto a=typeRegister->sharedAccessor("PO");
  a->callConstructor<PO>(0);
  a->callDestructor();
  a->callConstructor<PO>(1);
*/
  window = std::make_shared<ge::util::WindowObject>(1024,1024,false,idle,nullptr);

  glewInit();
  ge::gl::init();
  ge::gl::initShadersAndPrograms();
  ge::gl::setMediumAndGreaterDebugMessage();

  (*sh)();
  std::cerr<<(std::string&)*fl->getOutput()<<std::endl;
  auto sss=(std::shared_ptr<ge::gl::ShaderObject>&)*sh->getOutput();
  std::cerr<<sss->getSource()<<std::endl;
  glClearColor(0,1,0,1);

  /*
  auto aa = sharedFileExtensionExtractor(
      typeRegister,
      typeRegister->sharedAccessor("string"),
      std::make_shared<ge::core::Nullary>(typeRegister,std::string("triangle.gp")));

  auto cc = sharedTextFileLoader(typeRegister,"glslFileExtension.cfg",true);

  auto bb = sharedFileExtensionTranslator(
      typeRegister,
      typeRegister->sharedAccessor("u32"),
      cc,
      aa);


  (*bb)();
  std::cerr<<(unsigned)*bb->getOutput()<<std::endl;*/
  /*
  ((std::shared_ptr<ge::core::Function>&)*ff)=bb;
  (*((std::shared_ptr<ge::core::Function>&)*ff))();
  */
  //std::cerr<<(unsigned)(*((std::shared_ptr<ge::core::Function>&)*ff)->getOutput())<<std::endl;


  /*
     auto triangle = sharedProgramObjectFactory(
     sharedTextFileLoader("triangle.vp",typeRegister,true),
     sharedTextFileLoader("triangle.gp",typeRegister,true),
     sharedTextFileLoader("triangle.fp",typeRegister,true));
     triangle->bindOutput(typeRegister->sharedAccessor("ProgramObject"));
     (*triangle)();
     */

  /*
     auto tl=sharedTextFileLoader("main.cpp",typeRegister,true);
     (*tl)();
     std::cout<<(std::string&)(*tl->getOutput())<<std::endl;
     */


  window->mainLoop();
  return 0;
}

void idle(){
  glClear(GL_COLOR_BUFFER_BIT);
  window->swap();
}
