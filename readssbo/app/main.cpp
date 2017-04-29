#include"main.hpp"

void Init();
void Idle();

bool DisableAnttweakbar=true;
ge::util::ArgumentObject        *Args;
ge::util::WindowObject          *Window;
ge::gl::BufferObject            *readBuffer;
ge::gl::ProgramObject           *readProgram;
ge::gl::AsynchronousQueryObject *readQuery;

NDormon::GpuPerfApi*GPA;


int main(int Argc,char*Argv[]){
  Args=new ge::util::ArgumentObject(Argc,Argv);

  ShaderDir=Args->getArg("--shader-directory","shaders/");

  Window=new ge::util::WindowObject(
      256,256,false,
      Idle,nullptr,
      false,
      450,"core","debug");

  Init();
  Window->mainLoop();
  delete Window;
  delete Args;
  return 0;
}

const unsigned FLOATS_PER_THREAD = 32;
const unsigned COMPUTE_UNITS = 20;
const unsigned WAVEFRONT = 64;
const unsigned WAVEFRONTS_PER_COMPUTE_UNIT = 2048;
const unsigned READBUFFER_SIZE = WAVEFRONT*FLOATS_PER_THREAD*COMPUTE_UNITS*WAVEFRONTS_PER_COMPUTE_UNIT;
const unsigned WORKGROUP_SIZE = 64;
const unsigned NOF_WORKGROUPS = WAVEFRONT*COMPUTE_UNITS*WAVEFRONTS_PER_COMPUTE_UNIT/WORKGROUP_SIZE;

#define BYTES_PER_FLOAT 4
#define NANOSECONDS_IN_SECOND 1000000000
#define GIGA (1024*1024*1024)

void Idle(){
  glClearColor(0,0.1,0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  readProgram->use();
  readProgram->bindSSBO("readBuffer",readBuffer);
  glFinish();

#define ITERATIONS 50
  unsigned long long elapsedTime=0;
  if(Window->isKeyOn('n')){
    GPA->beginSession();
    for(unsigned p=0;p<GPA->getNumPasses();++p){
      GPA->beginPass();
      GPA->beginSample(0);

      glDispatchCompute(NOF_WORKGROUPS,1,1);

      GPA->endSample();
      GPA->endPass();
    }
    GPA->endSession();
    std::cout<<GPA->getResults()<<std::endl;

  }else{
    for(unsigned i=0;i<ITERATIONS;++i){
      readQuery->begin();
      glDispatchCompute(NOF_WORKGROUPS,1,1);
      readQuery->end();
      elapsedTime+=readQuery->getui64();
      //glFinish();
    }
  }

  double throughput=
    (double)ITERATIONS*
    BYTES_PER_FLOAT*
    READBUFFER_SIZE*
    NANOSECONDS_IN_SECOND/
    (double)(elapsedTime);
  std::cout<<throughput/GIGA<<" GB/second"<<std::endl;
  Window->swap();
}

void Init(){
  glewExperimental=GL_TRUE;
  glewInit();

  GPA=new NDormon::GpuPerfApi(Window->getContext());
  GPA->enableComputeShader();

  ge::gl::initShadersAndPrograms();
  ge::gl::setDefaultDebugMessage();

  readProgram = new ge::gl::ProgramObject(
      ShaderDir+"read.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE"         ,(int)WORKGROUP_SIZE         )+
      ge::gl::ShaderObject::define("FLOATS_PER_THREAD"      ,(int)FLOATS_PER_THREAD      )+
      "");

  std::cout<<READBUFFER_SIZE*sizeof(float)/1024/1024<<"MB"<<std::endl;
  readBuffer = new ge::gl::BufferObject(
      READBUFFER_SIZE*sizeof(float));

  readBuffer->clear(GL_R32F,GL_RED,GL_FLOAT);
  
  readQuery=new ge::gl::AsynchronousQueryObject(
      GL_TIME_ELAPSED,
      GL_QUERY_RESULT,
      ge::gl::AsynchronousQueryObject::UINT64);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
}


