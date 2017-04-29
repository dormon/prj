#include<GL/gl.h>
#include<GL/glext.h>

#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

#include<cassert>
GLhalfNV boobobobo;
#include<SDL2/SDL.h>

#include<geGL/Export.h>

#include<geGL/OpenGLFunctionTable.h>
#include<geGL/OpenGLFunctionProvider.h>
#include<geGL/LoadOpenGLFunctions.h>
#include<geGL/CheckOpenGLFunctions.h>

class OpenGLObject: protected ge::gl::OpenGLFunctionProvider{
  protected:
    GLuint _id;
  public:
    OpenGLObject(std::shared_ptr<ge::gl::OpenGLFunctionTable>const&table):ge::gl::OpenGLFunctionProvider(table){
      this->_id = 0;
    }
    inline GLuint getId()const{return this->_id;}
};

class BufferObject: protected OpenGLObject{
  public:
    BufferObject(std::shared_ptr<ge::gl::OpenGLFunctionTable>const&table,GLsizeiptr size):OpenGLObject(table){
      this->glCreateBuffers(1,&this->_id);
      this->glNamedBufferData(this->_id,size,nullptr,GL_STATIC_DRAW);
    }
    void bind(GLenum target)const{
      this->glBindBuffer(target,this->_id);
    }
};

int main(){
  GLhalfNV asd;
  asd = 32;
  std::cout<<asd<<std::endl;
  uint32_t version     = 450;
  uint32_t profile     = SDL_GL_CONTEXT_PROFILE_CORE;
  uint32_t contextFlag = SDL_GL_CONTEXT_DEBUG_FLAG;
  SDL_Init(SDL_INIT_VIDEO);//initialise video
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,contextFlag     );
  auto window = SDL_CreateWindow(
        "SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  auto context = SDL_GL_CreateContext(window);

  auto glTable = std::make_shared<ge::gl::OpenGLFunctionTable>();
  ge::gl::loadOpenGLFunctions(glTable,(ge::gl::GET_PROC_ADDRESS)SDL_GL_GetProcAddress);
  ge::gl::checkOpenGLFunctions(glTable);
  //SDL_GL_GetProcAddress
  BufferObject vbo(glTable,100);
  SDL_Event E;//event
  bool running = true;
  while(running){//endless loop
    while(SDL_PollEvent(&E)){//loop over events
      switch(E.type){//switch over event types
        case SDL_QUIT://quit main loop
          running=false;//stop running
          break;//break quit case
        default://other events
          break;//break other events case
      }
    }
    //do idle
    glTable->glClearColor(0,1,0,1);
    glTable->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glTable->glGetProgramivNV(0,0,0);
    SDL_GL_SwapWindow(window);
  }
  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}


