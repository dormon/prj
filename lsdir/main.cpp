#include<iostream>
#include"LsDirInterface.h"
#include"LibraryLoader.h"
#include"ObjectLoader.h"

#include"GetProcAddressInterface.h"

#include<SDL2/SDL.h>

int main(int argc,char*argv[]){
  argc=argc;
  argv=argv;
  ObjectLoader*objLoader=(ObjectLoader*)getLoader();
  LsDir*ls=(LsDir*)objLoader->getObject("./linuxLsDir.so");
  ls->cd("/home/dormon/prj/lsdir");

  //SDL_Init(SDL_INIT_VIDEO);
  //SDL_CreateWindow("test",0,0,100,100,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

  //GetProcAddress*getProcAddress = (GetProcAddress*)objLoader->getObject("./SDL2GetProcAddress.so");
  //if(getProcAddress->get("glUseProgram")==NULL)std::cout<<"glUseProgram == NULL"<<std::endl;

  std::string com;
  for(;;){
    std::cout<<"> ";
    std::cin>>com;
    if(com=="exit")break;
    if(com=="pwd"){
      std::cout<<ls->getCurrentDir()<<std::endl;
      continue;
    }
    if(com=="ls"){
      for(unsigned i=0;i<ls->getNofFiles();++i)
        std::cout<<ls->getFile(i)<<std::endl;
      for(unsigned i=0;i<ls->getNofDirs();++i)
        std::cout<<ls->getDir(i)<<"/"<<std::endl;
    }else{
      ls->cd(com);
    }
  }
  std::cout<<ls->getCurrentDir()<<std::endl;
  for(unsigned i=0;i<ls->getNofFiles();++i)
    std::cout<<ls->getFile(i)<<std::endl;

  delete ls;
  //delete getProcAddress;
  delete objLoader;
 
  /*
  LibraryLoader*libLoader=getLoader();
  unsigned lsdir=libLoader->load("./linuxLsDir.so");
  LsDir*(*getLsDirObject)();
  *(void**)(&getLsDirObject) = libLoader->get(lsdir,"getObject");
  LsDir*ls=getLsDirObject();
  ls->cd(".");
  for(unsigned i=0;i<ls->getNofFiles();++i)
    std::cout<<ls->getFile(i)<<std::endl;

  delete ls;
  delete libLoader;
  */

  //LsDir*lsDir=NULL;
  //lsDir->getFile(0);
  return 0;
}
