#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geUtil/WindowObject.h>
#include<geUtil/copyArgumentManager2Namespace.h>


std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;
std::shared_ptr<ge::util::sim::NamespaceWithUsers>sData = nullptr;
ge::util::WindowObject*window = nullptr;


int main(int argc,char*argv[]){
  typeRegister = std::make_shared<ge::core::TypeRegister>();
  ge::util::ArgumentManager*argm = new ge::util::ArgumentManager(argc-1,argv+1);
  sData = std::make_shared<ge::util::sim::NamespaceWithUsers>("*");
  ge::util::sim::copyArgumentManager2Namespace(sData,argm,typeRegister);
  window = new ge::util::WindowObject(
      sData->get<unsigned[]>("window.size",{1024,1024})[0],
      sData->get<unsigned[]>("window.size",{1024,1024})[1],
      sData->get<bool      >("window.fullscreen",false),
      nullptr,
      nullptr,
      true);
  window->mainLoop();
  delete window;
  delete argm;
  return 0;
}
