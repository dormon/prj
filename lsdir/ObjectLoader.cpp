#include"ObjectLoader.h"

void*ObjectLoader::getObject(std::string name){
  unsigned lib=this->load(name);
  void*(*objectFactory)();
  *(void**)(&objectFactory)=this->get(lib,"getObject");
  return objectFactory();
}
