#include<iostream>
#include<dlfcn.h>
#include"base.h"

class Explorer{
  protected:
  public:
    Explorer(std::string dir="./");

};

int main(int argc,char*argv[]){
  void* myso = dlopen("./test.so", RTLD_LAZY);
  if(!myso){
    std::cerr<<dlerror()<<std::endl;
    return 0;
  }
  void (*fce)();
  
  *(void**)(&fce) = dlsym(myso, "printAhoj");
  if(!fce){
    std::cerr<<dlerror()<<std::endl;
    return 0;
  }

  fce();

  
  Loader*(*fce2)();
  *(void**)(&fce2) = dlsym(myso,"getLoader");
  if(!fce2){
    std::cerr<<dlerror()<<std::endl;
    return 0;
  }

  Loader*ldr=fce2();
  ldr->print();
  delete ldr;
  
  dlclose(myso);

  return 0;
}
