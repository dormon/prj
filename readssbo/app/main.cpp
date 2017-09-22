#include"main.hpp"

using namespace std;
using namespace ge::gl;
using namespace ge::ad;

const size_t BLOCKS_PER_WORKGROUP = 8   ;
const size_t BLOCK_STRIDE         = 64  ;//8192;
const size_t BLOCK_SIZE           = 64  ;
const size_t WORKGROUP_SIZE       = 64  ;

const size_t ITERATIONS              = 50                                                                  ;
const size_t BYTES_PER_FLOAT         = sizeof(float)                                                       ;
const size_t NANOSECONDS_IN_SECOND   = 1000000000                                                          ;
const size_t GIGA                    = 1 << 30                                                             ;
const size_t MEGA                    = 1 << 20                                                             ;
const size_t COMPUTE_UNITS           = 20                                                                  ;
const size_t GROUPS_PER_COMPUTE_UNIT = 40*32                                                                  ;
const size_t NOF_WORKGROUPS          = COMPUTE_UNITS * GROUPS_PER_COMPUTE_UNIT                             ;

std::string const readSource = R".(
#ifndef WORKGROUP_SIZE
#define WORKGROUP_SIZE 32
#endif//WORKGROUP_SIZE

#ifndef BLOCK_STRIDE
#define BLOCK_STRIDE 1
#endif//BLOCK_STRIDE

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1
#endif//BLOCK_SIZE

#ifndef BLOCKS_PER_WORKGROUP
#define BLOCKS_PER_WORKGROUP 4
#endif//BLOCKS_PER_WORKGROUP

layout(std430,binding=0)buffer Data{float readBuffer[];};

layout(local_size_x=WORKGROUP_SIZE)in;

void main(){
  float data = 0;

#define WORKGROUPS_READS_PER_BLOCK  (BLOCK_SIZE/WORKGROUP_SIZE)

  for(uint i=0;i<BLOCKS_PER_WORKGROUP;++i)
    for(uint j=0;j<WORKGROUPS_READS_PER_BLOCK;++j)
      data += readBuffer[

        gl_WorkGroupID.x*BLOCKS_PER_WORKGROUP*BLOCK_STRIDE+
        i*BLOCK_STRIDE+
        j*WORKGROUP_SIZE+
        gl_LocalInvocationID.x];


  if(data==3233.323f)
    readBuffer[gl_GlobalInvocationID.x]=0.f;
}
).";

int main(int argc,char*argv[]){
  static_cast<void>(argc);
  static_cast<void>(argv);

  auto mainLoop = make_shared<SDLMainLoop>();
  auto window   = make_shared<SDLWindow  >();

  window->createContext("rendering");
  mainLoop->addWindow("mainWindow", window);
  bool keyDown[256];
  window->setEventCallback(SDL_KEYDOWN,[&](SDL_Event const&e){
    keyDown[e.key.keysym.sym%256] = true;
    return true;
  });
  window->setEventCallback(SDL_KEYUP,[&](SDL_Event const&e){
    keyDown[e.key.keysym.sym%256] = false;
    return true;
  });

  ge::gl::init(SDL_GL_GetProcAddress);
  setHighDebugMessage();

  NDormon::GpuPerfApi*GPA;


  GPA=new NDormon::GpuPerfApi(window->getContext("rendering"));
  GPA->enableComputeShader();

  auto readQuery = make_shared<AsynchronousQuery>(
      GL_TIME_ELAPSED,
      GL_QUERY_RESULT,
      ge::gl::AsynchronousQuery::UINT64);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);


  /*
  auto measure = [&](size_t size,size_t stride,size_t blocksPerWorkgroup,size_t workGroupSize){
    const size_t readSize   = BYTES_PER_FLOAT * BLOCK_SIZE * BLOCKS_PER_WORKGROUP * NOF_WORKGROUPS;
    const size_t bufferSize = (readSize * BLOCK_STRIDE) / BLOCK_SIZE                              ;

    auto cs = make_shared<Shader>(GL_COMPUTE_SHADER,
        "#version 450\n",
        ge::gl::Shader::define("WORKGROUP_SIZE"      ,(int)workGroupSize     )+
        ge::gl::Shader::define("BLOCKS_PER_WORKGROUP",(int)blocksPerWorkgroup)+
        ge::gl::Shader::define("BLOCK_STRIDE"        ,(int)stride            )+
        ge::gl::Shader::define("BLOCK_SIZE"          ,(int)size              )+
        readSource);

    auto readProgram = make_shared<Program>(cs);

    std::cout<<"READ  : "<<readSize  /MEGA<<"MB"<<std::endl;
    std::cout<<"BUFFER: "<<bufferSize/MEGA<<"MB"<<std::endl;

    auto readBuffer = make_shared<Buffer>(bufferSize);
    readBuffer->clear(GL_R32F,GL_RED,GL_FLOAT);

    readProgram->use();
    readProgram->bindBuffer("readBuffer",readBuffer);
    glFinish();
    size_t elapsedTime = 0;
    for(unsigned j=0;j<10;++j){
      elapsedTime = 0;
      for(unsigned i=0;i<ITERATIONS;++i){
        readQuery->begin();
        glDispatchCompute(NOF_WORKGROUPS,1,1);
        readQuery->end();
        elapsedTime+=readQuery->getui64();
      }
    }

    double throughput=
      (double)ITERATIONS*
      readSize*
      NANOSECONDS_IN_SECOND/
      (double)(elapsedTime);
    std::cout<<throughput/GIGA<<" GB/second"<<std::endl;
  };

  measure(64,64  ,32,64);
  //measure(16,16  ,8,16);
  //measure(16,32  ,8,16);
  //measure(16,64  ,8,16);
  //measure(16,128 ,8,16);
  //measure(16,512 ,8,16);
  //measure(16,1024,8,16);
  //measure(16,2048,8,16);
  //measure(16,4096,8,16);
  //measure(16,8192,8,16);
  // */



  //*
  const size_t READ_SIZE               = BYTES_PER_FLOAT * BLOCK_SIZE * BLOCKS_PER_WORKGROUP * NOF_WORKGROUPS;
  const size_t BUFFER_SIZE             = (READ_SIZE * BLOCK_STRIDE) / BLOCK_SIZE                             ;

  auto cs = make_shared<Shader>(GL_COMPUTE_SHADER,
      "#version 450\n",
      ge::gl::Shader::define("WORKGROUP_SIZE"      ,(int)WORKGROUP_SIZE      )+
      ge::gl::Shader::define("BLOCKS_PER_WORKGROUP",(int)BLOCKS_PER_WORKGROUP)+
      ge::gl::Shader::define("BLOCK_STRIDE"        ,(int)BLOCK_STRIDE        )+
      ge::gl::Shader::define("BLOCK_SIZE"          ,(int)BLOCK_SIZE          )+
      readSource);

  auto readProgram = make_shared<Program>(cs);

  //std::cout<<"READ  : "<<READ_SIZE  /MEGA<<"MB"<<std::endl;
  //std::cout<<"BUFFER: "<<BUFFER_SIZE/MEGA<<"MB"<<std::endl;

  auto readBuffer = make_shared<Buffer>(BUFFER_SIZE);
  readBuffer->clear(GL_R32F,GL_RED,GL_FLOAT);

  mainLoop->setIdleCallback([&]() {
      glClearColor(0,0.1,0,1);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      readProgram->use();
      readProgram->bindBuffer("readBuffer",readBuffer);
      glFinish();

      unsigned long long elapsedTime=0;
      if(keyDown[static_cast<size_t>('n')]){
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
        READ_SIZE*
        NANOSECONDS_IN_SECOND/
        (double)(elapsedTime);
      std::cout<<throughput/GIGA<<" GB/second"<<std::endl;
      window->swap();
  });

  (*mainLoop)();
  // */

  return EXIT_SUCCESS;
}

