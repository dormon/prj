#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<GL/glew.h>
#include<SDL2/SDL.h>

void debugFce(
    GLenum        source,
    GLenum        type,
    GLuint        /*id*/,
    GLenum        severity,
    GLsizei       /*length*/,
    const GLchar *message,
    void*        /*userParam*/){
  std::cerr<<
    "source: "   <<source  <<
    " type: "    <<type    <<
    " severity: "<<severity<<
    " : "        <<message <<std::endl;
}


int main(){
  unsigned version=450;
  SDL_GLprofile profile = SDL_GL_CONTEXT_PROFILE_CORE;
  SDL_GLcontextFlag contextFlag = SDL_GL_CONTEXT_DEBUG_FLAG;

  SDL_Init(SDL_INIT_VIDEO);//initialise video
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100    );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile         );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS        ,contextFlag     );

  auto window = SDL_CreateWindow("",0,0,1,1,SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);
  auto context = SDL_GL_CreateContext(window);
  glewExperimental=GL_TRUE;
  glewInit();
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)debugFce,nullptr);

  GLint nofExtensions = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &nofExtensions);
  for(int i=0;i<nofExtensions;++i)
    std::cout<<glGetStringi(GL_EXTENSIONS,i)<<std::endl;
  SDL_GL_DeleteContext(context);
  return 0;
}
