#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<tuple>
#include<string>
#include<cassert>
#include<sstream>
#include<cstring>
#include<utility>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

inline std::string value2str(glm::vec3 const&a){
  std::stringstream ss;
  ss<<"glm::vec3("<<a.x<<","<<a.y<<","<<a.z<<")";
  return ss.str();
}


#include"TypeRegister.h"
#include"Resource.h"
#include"Nullary.h"
#include"Reference.h"
#include"Dereference.h"
#include"CreateFunction.h"
#include"ValuePrinter.h"

template<>inline std::string keyword<glm::vec3>(){
  return keyword<float[3]>();
}

template<>inline std::string keyword<glm::mat4>(){
  return keyword<float[16]>();
}

int32_t add(int32_t a,int32_t b){
  PRINT_CALL_STACK(a,b);
  return a+b;
}

int32_t addP(int32_t*a,int32_t*b){
  PRINT_CALL_STACK(a,b);
  return *a+*b;
}

float ddot(glm::vec3 const&a,glm::vec3 const&b){
  return glm::dot<float>(a,b);
}

void printInt(int32_t a){
  std::cout<<a<<std::endl;
}

int32_t*allocInts(){
  return new int32_t[10];
}

void freeInts(int32_t*d){
  delete[]d;
}

class ASD{
  public:
    int i;
};

int main(){
  ASD aaaa;
  aaaa.i=100;
  std::cout<<value2str(aaaa)<<std::endl;
  auto tr = std::make_shared<TypeRegister>();

  tr->addAnyType();
  tr->addType<Any()>();
  tr->addType<void>();
  tr->addType<int32_t>();
  tr->addType<int32_t*>();
  tr->addType<int32_t[3]>();
  tr->addType<int32_t[5][6]>();
  tr->addType<int32_t(int32_t,int32_t,int32_t)>();
  tr->addType<int32_t()>();
  tr->addType<float>();
  tr->addType<float[3]>();
  tr->addType<float[16]>();
  //tr->addType<decltype(add)>();

  for(size_t i=0;i<tr->nofTypes();++i){
    std::cout<<tr->typeIdName(i)<<": ";
    std::cout<<tr->str(i)<<std::endl;
  }

  auto res0 = tr->createResource<int32_t>();
  auto res1 = tr->createResource<int32_t[3]>();
  auto res2 = tr->createResource<int32_t[5][6]>();
  //*(int32_t*)res0->getData() = 32;
  //*(int32_t*)*res0 = (int32_t)32;
  ___;
  (void)(int32_t       )*res0;
  ___;
  (void)(int32_t const )*res0;
  ___;
  (void)(int32_t      &)*res0;
  ___;
  (void)(int32_t const&)*res0;
  ___;



  auto a = std::make_shared<Nullary>(tr,(int32_t)10);
  auto b = std::make_shared<Nullary>(tr,(int32_t)22);
  auto addFce = createFce(tr,add);
  addFce->bindInput(0,a);
  addFce->bindInput(1,b);
  addFce->bindOutput(tr->createResource<int32_t>());
  auto printAddFce = createFce(tr,printInt);
  printAddFce->bindInput(0,addFce);
  //(*addFce)();
  //std::cout<<addFce->getOutputData()->operator int32_t&()<<std::endl;
  (*printAddFce)();

  auto ap = std::make_shared<Reference<int32_t>>(tr);
  auto bp = std::make_shared<Reference<int32_t>>(tr);
  ap->bindInput(0,a);
  ap->bindOutput(tr->createResource<int32_t*>());
  bp->bindInput(0,b);
  bp->bindOutput(tr->createResource<int32_t*>());

  auto addPFce = createFce(tr,addP);
  addPFce->bindInput(0,ap);
  addPFce->bindInput(1,bp);
  addPFce->bindOutput(tr->createResource<int32_t>());
  (*addPFce)();

  std::cout<<(int32_t)(*addPFce->getOutputData())<<std::endl;

  auto dot = createFce(tr,glm::dot<float>);
  auto va =std::make_shared<Nullary>(tr,glm::vec3(101.1f,1.f,1.f));
  auto vb =std::make_shared<Nullary>(tr,glm::vec3(1.f,1.f,1.f));
  dot->bindInput(0,va);
  dot->bindInput(1,vb);
  dot->bindOutput(tr->createResource<float>());
  (*dot)();
  std::cout<<(float&)*dot->getOutputData()<<std::endl;


  auto allocIntsFce = createFce(tr,allocInts);
  auto ints = tr->createResource<int32_t*>();
  allocIntsFce->bindOutput(ints);
  
  auto freeIntsFce = createFce(tr,freeInts);
  freeIntsFce->bindInput(0,allocIntsFce);

  //(*allocIntsFce)();
  (*freeIntsFce)();

  std::cout<<keyword<int32_t const*const>()<<std::endl;
  return 0;
}





