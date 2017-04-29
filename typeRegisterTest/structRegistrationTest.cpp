#include"structRegistrationTest.h"

#include<geCore/TypeRegister.h>

using namespace ge::core;

void structRegistrationTest(){
  ___;
  TypeRegister*typeRegister=new TypeRegister();
  ___;
  typeRegister->addType("a0",TypeRegister::STRUCT,1,2000);
  ___;
  typeRegister->addType("a1",TypeRegister::STRUCT,1);
  ___;
  typeRegister->addType("a2",TypeRegister::STRUCT,1);
  ___;
  typeRegister->addType("a22",TypeRegister::STRUCT,2,TypeRegister::STRUCT,2,TypeRegister::I32);
  ___;
  typeRegister->addType("a3",TypeRegister::STRUCT,0,"i32");
  ___;
  typeRegister->addType("a4",TypeRegister::STRUCT,1,"i32");
  ___;
  typeRegister->addType("a5",TypeRegister::STRUCT,1,TypeRegister::I8);
  ___;
  typeRegister->addType("a6",TypeRegister::STRUCT,1,TypeRegister::STRUCT,1,2000);
  ___;
  typeRegister->addType("a7",TypeRegister::STRUCT,1,TypeRegister::STRUCT,300);
  ___;
  typeRegister->addType("a8",TypeRegister::STRUCT,1,TypeRegister::STRUCT);
  ___;
  typeRegister->addType("a9",TypeRegister::STRUCT,1,TypeRegister::STRUCT,1,TypeRegister::I32);
  ___;
  std::vector<unsigned>finalDesc={{1,2,3,4,5,6,7,8,9,10,11,12,13,14,2,23,14,3,23,14,4,23,14,2,27,14,3,27,14,4,27,14,2,29,14,3,29,14,4,29,15,1,23,15,1,21,15,1,41}};
  for(unsigned i=0;i<typeRegister->getNofDescriptionUints();++i)
    if(typeRegister->getDescription(i)!=finalDesc[i])
      std::cerr<<"final description vector is different!"<<std::endl;

  delete typeRegister;
  ___;
}
