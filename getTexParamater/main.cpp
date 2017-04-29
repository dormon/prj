#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<SDL2/SDL.h>
#include<GL/glew.h>
#include"sdlWindow.h"
#include<geGL/geGL.h>

std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;
EventProc*mainLoop=nullptr;

void primIdle(){
  mainLoop->makeCurrent("prim");
  glClearColor(0,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  mainLoop->swap("prim");
}

void secIdle(){
  mainLoop->makeCurrent("sec");
  glClearColor(0,0,1,1);
  glClear(GL_COLOR_BUFFER_BIT);
  mainLoop->swap("sec");
}

void primKeyDown(ge::core::Accessor const&data){
  std::cout<<data.data2Str()<<std::endl;
}

static const std::vector<GLenum>textureLevelPnames={
  GL_TEXTURE_WIDTH                 ,
  GL_TEXTURE_HEIGHT                ,
  GL_TEXTURE_DEPTH                 ,
  GL_TEXTURE_FIXED_SAMPLE_LOCATIONS,
  GL_TEXTURE_INTERNAL_FORMAT       ,
  GL_TEXTURE_SHARED_SIZE           ,
  GL_TEXTURE_COMPRESSED            ,
  GL_TEXTURE_COMPRESSED_IMAGE_SIZE ,
  GL_TEXTURE_SAMPLES               ,
  GL_TEXTURE_BUFFER_OFFSET         ,
  GL_TEXTURE_BUFFER_SIZE           ,
  GL_TEXTURE_RED_SIZE              ,
  GL_TEXTURE_GREEN_SIZE            ,
  GL_TEXTURE_BLUE_SIZE             ,
  GL_TEXTURE_ALPHA_SIZE            ,
  GL_TEXTURE_DEPTH_SIZE            ,
  GL_TEXTURE_RED_TYPE              ,
  GL_TEXTURE_GREEN_TYPE            ,
  GL_TEXTURE_BLUE_TYPE             ,
  GL_TEXTURE_ALPHA_TYPE            ,
  GL_TEXTURE_DEPTH_TYPE            ,
};

static const std::vector<std::string>textureLevelPnameNames={
  "GL_TEXTURE_WIDTH"                 ,
  "GL_TEXTURE_HEIGHT"                ,
  "GL_TEXTURE_DEPTH"                 ,
  "GL_TEXTURE_FIXED_SAMPLE_LOCATIONS",
  "GL_TEXTURE_INTERNAL_FORMAT"       ,
  "GL_TEXTURE_SHARED_SIZE"           ,
  "GL_TEXTURE_COMPRESSED"            ,
  "GL_TEXTURE_COMPRESSED_IMAGE_SIZE" ,
  "GL_TEXTURE_SAMPLES"               ,
  "GL_TEXTURE_BUFFER_OFFSET"         ,
  "GL_TEXTURE_BUFFER_SIZE"           ,
  "GL_TEXTURE_RED_SIZE"              ,
  "GL_TEXTURE_GREEN_SIZE"            ,
  "GL_TEXTURE_BLUE_SIZE"             ,
  "GL_TEXTURE_ALPHA_SIZE"            ,
  "GL_TEXTURE_DEPTH_SIZE"            ,
  "GL_TEXTURE_RED_TYPE"              ,
  "GL_TEXTURE_GREEN_TYPE"            ,
  "GL_TEXTURE_BLUE_TYPE"             ,
  "GL_TEXTURE_ALPHA_TYPE"            ,
  "GL_TEXTURE_DEPTH_TYPE"            ,
};

static const std::vector<GLenum>textureTargets={
  GL_TEXTURE_1D                        ,
  GL_TEXTURE_2D                        ,
  GL_TEXTURE_3D                        ,
  GL_TEXTURE_BUFFER                    ,
  GL_TEXTURE_RECTANGLE                 ,
  GL_TEXTURE_2D_MULTISAMPLE            ,
  GL_TEXTURE_1D_ARRAY                  ,
  GL_TEXTURE_2D_ARRAY                  ,
  GL_TEXTURE_CUBE_MAP_ARRAY            ,
  GL_TEXTURE_2D_MULTISAMPLE_ARRAY      ,
  GL_PROXY_TEXTURE_1D                  ,
  GL_PROXY_TEXTURE_2D                  ,
  GL_PROXY_TEXTURE_3D                  ,
  GL_PROXY_TEXTURE_CUBE_MAP            ,
  GL_PROXY_TEXTURE_1D_ARRAY            ,
  GL_PROXY_TEXTURE_1D_ARRAY            ,
  GL_PROXY_TEXTURE_2D_ARRAY            ,
  GL_PROXY_TEXTURE_CUBE_MAP_ARRAY      ,
  GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

static const std::vector<std::string>textureTargetNames={
  "GL_TEXTURE_1D"                        ,
  "GL_TEXTURE_2D"                        ,
  "GL_TEXTURE_3D"                        ,
  "GL_TEXTURE_BUFFER"                    ,
  "GL_TEXTURE_RECTANGLE"                 ,
  "GL_TEXTURE_2D_MULTISAMPLE"            ,
  "GL_TEXTURE_1D_ARRAY"                  ,
  "GL_TEXTURE_2D_ARRAY"                  ,
  "GL_TEXTURE_CUBE_MAP_ARRAY"            ,
  "GL_TEXTURE_2D_MULTISAMPLE_ARRAY"      ,
  "GL_PROXY_TEXTURE_1D"                  ,
  "GL_PROXY_TEXTURE_2D"                  ,
  "GL_PROXY_TEXTURE_3D"                  ,
  "GL_PROXY_TEXTURE_CUBE_MAP"            ,
  "GL_PROXY_TEXTURE_1D_ARRAY"            ,
  "GL_PROXY_TEXTURE_1D_ARRAY"            ,
  "GL_PROXY_TEXTURE_2D_ARRAY"            ,
  "GL_PROXY_TEXTURE_CUBE_MAP_ARRAY"      ,
  "GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY",
};

#define DEF_2STRFCE(NAME,IDS,NAMES)\
  std::string NAME(GLenum param){\
    unsigned counter=0;\
    for(auto x:IDS)\
      if(x==param)return NAMES[counter];\
      else counter++;\
    return "unknown";\
  }
 
DEF_2STRFCE(textureTarget2Str    ,textureTargets    ,textureTargetNames    );
DEF_2STRFCE(textureLevelPname2Str,textureLevelPnames,textureLevelPnameNames);

void printLevelInfo(GLenum target,GLint level,GLenum pname){
  GLint d;
  glGetTexLevelParameteriv(target,level,pname,&d);
  std::cout<<ge::gl::translateTextureTarget(target)<<" "<<level<<" "<<textureLevelPname2Str(pname)<<": "<<d<<std::endl;
}

int main(){
  typeRegister = std::make_shared<ge::core::TypeRegister>();
  mainLoop=new SDLEventProc(typeRegister);
  mainLoop->addWindow("prim",std::make_shared<SDLWindow>(1000,1000));
  mainLoop->addWindow("sec",std::make_shared<SDLWindow>(100,100));
  std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("prim"))->createContext(450,SDLWindow::CORE,SDLWindow::DEBUG);
  std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("sec"))->setContext(*std::dynamic_pointer_cast<SDLWindow>(mainLoop->getWindow("prim")));
  mainLoop->setCallback("prim","idle"   ,primIdle   );
  mainLoop->setCallback("prim","keyDown",primKeyDown);
  mainLoop->setCallback("sec" ,"idle"   ,secIdle    );
  glewInit();
  ge::gl::init();
  ge::gl::setDefaultDebugMessage();
  //GLuint t3D;
  //GLuint t2D;
  GLuint t1D;
  glCreateTextures(GL_TEXTURE_2D,1,&t1D);
  glTextureStorage2D(t1D,10,GL_RGBA8UI,500,323);
  glGenerateTextureMipmap(t1D);
  for(GLint i=0;i<10;++i){
    GLint w,h;
    glGetTextureLevelParameteriv(t1D,i,GL_TEXTURE_WIDTH,&w);
    glGetTextureLevelParameteriv(t1D,i,GL_TEXTURE_HEIGHT,&h);
    std::cout<<w<<"x"<<h<<std::endl;
  }

  //glBindTextureUnit(0,0);
  GLint w;
  glGetTextureParameteriv(t1D,GL_TEXTURE_TARGET,&w);
  std::cout<<textureTarget2Str(w)<<std::endl;
  /*
  for(auto x:textureTargets){
    GLuint texture;
    glCreateTextures(x,1,&texture);
    GLenum y;
    glGetTextureParameteriv(texture,GL_TEXTURE_TARGET,(GLint*)&y);
    std::cout<<textureTarget2Str(x)<<" != "<<textureTarget2Str(y)<<std::endl;
    glDeleteTextures(1,&texture);
  }*/
  //for(auto x:textureLevelPnames)
  //  printLevelInfo(GL_TEXTURE_1D,1,x);
  //(*mainLoop)();
  delete mainLoop;
  return 0;
}
