#include<iostream>
#include"LsDirInterface.h"
#include<geUtil/LibraryLoader/LibraryLoader.h>
#include<geUtil/LibraryLoader/LibraryObjectLoader.h>

#include"GetProcAddressInterface.h"

#include<SDL2/SDL.h>
#include<geUtil/LibraryLoader/RuntimeClassInterface.h>


int main(int argc,char*argv[]){
  argc=argc;
  argv=argv;

  std::shared_ptr<ge::core::TypeRegister>typeRegister=std::make_shared<ge::core::TypeRegister>();

  ge::util::ObjectLoader*objLoader=(ge::util::ObjectLoader*)ge::util::getLoader();
  ge::util::RuntimeClassInterface*lsI=objLoader->getInterface("./linuxLsDir.so",typeRegister);

  void*ls=lsI->construct();
  std::cout<<lsI->dir();
  ge::core::Accessor dir=typeRegister->allocAccessor("string");
  //dir=std::string("/home/dormon/prj/lsdir");
  //lsI->call(ls,"cd",dir);
  lsI->call(ls,"cd",std::string("/home/dormon/prj/lsdir"));
  //dir.free();

  std::string com;
  for(;;){
    std::cout<<"> ";
    std::cin>>com;
    if(com=="exit")break;
    if(com=="pwd"){
      ge::core::Accessor dir=typeRegister->allocAccessor("string");
      lsI->call(ls,"getCurrentDir",dir);
      std::cout<<(std::string&)dir<<std::endl;
      continue;
    }
    if(com=="ls"){
      ge::core::Accessor item=typeRegister->allocAccessor("string");
      ge::core::Accessor index=typeRegister->allocAccessor("u32");
      ge::core::Accessor nof=typeRegister->allocAccessor("u32");

      lsI->call(ls,"getNofFiles",nof);
      for(unsigned i=0;i<(unsigned)nof;++i){
        index=i;
        lsI->call(ls,"getFile",item,index);
        std::cout<<(std::string&)item<<std::endl;
      }

      lsI->call(ls,"getNofDirs",nof);
      for(unsigned i=0;i<(unsigned)nof;++i){
        index=i;
        lsI->call(ls,"getDir",item,index);
        std::cout<<(std::string&)item<<std::endl;
      }

    }else{
      ge::core::Accessor command=typeRegister->allocAccessor("string");
      command=com;
      lsI->call(ls,"cd",command);
    }
  }
  lsI->destroy(ls);
  delete lsI;
  delete objLoader;
 
  return 0;
}
