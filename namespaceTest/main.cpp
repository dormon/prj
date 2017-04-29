#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geUtil/ArgumentManager.h>
#include<geCore/TypeRegister.h>

#include<geUtil/NamespaceWithUsers.h>
#include<geUtil/copyArgumentManager2Namespace.h>
//#include"NamespaceWithUsers.h"
//#include"copyArgumentManager2Namespace.h"

std::shared_ptr<ge::util::sim::NamespaceWithUsers>sData        = nullptr;
ge::util::ArgumentManager*                        argm         = nullptr;
std::shared_ptr<ge::core::TypeRegister>           typeRegister = nullptr;

int main(int argc,char*argv[]){
  typeRegister = std::make_shared<ge::core::TypeRegister>();
  sData = std::make_shared<ge::util::sim::NamespaceWithUsers>("*");
  argm = new ge::util::ArgumentManager(argc-1,argv+1);
  ge::util::sim::copyArgumentManager2Namespace(sData,argm,typeRegister);
  std::cout<<sData->toStr(0)<<std::endl;
  delete argm;
  return 0;
}
