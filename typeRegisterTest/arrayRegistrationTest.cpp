#include"arrayRegistrationTest.h"

#include<geCore/TypeRegister.h>

using namespace ge::core;

void arrayRegistrationTest(){
  return;
  ___;
  TypeRegister*typeRegister=new TypeRegister();
  ___;
  typeRegister->addType("a",TypeRegister::ARRAY,300,2000);
  ___;
  typeRegister->addType("a",TypeRegister::ARRAY,300);
  ___;
  typeRegister->addType("a",TypeRegister::ARRAY);
  ___;
  typeRegister->addType("a");
  ___;
  typeRegister->addType("a0",TypeRegister::ARRAY,100,(unsigned)TypeRegister::I32);
  ___;
  typeRegister->addType("a1",TypeRegister::ARRAY,90,"i32");
  ___;
  typeRegister->addType("a2",TypeRegister::ARRAY,80,TypeRegister::ARRAY,8,2000);
  ___;
  typeRegister->addType("a3",TypeRegister::ARRAY,80,TypeRegister::ARRAY,8);
  ___;
  typeRegister->addType("a4",TypeRegister::ARRAY,80,TypeRegister::ARRAY);
  ___;
  typeRegister->addType("a5",TypeRegister::ARRAY,80,TypeRegister::ARRAY,80,TypeRegister::I32);
  ___;
  std::vector<unsigned>finalDesc={{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 23, 14, 3, 23, 14, 4, 23, 14, 2, 27, 14, 3, 27, 14, 4, 27, 14, 2, 29, 14, 3, 29, 14, 4, 29, 14, 100, 23, 14, 90, 23, 14, 80, 23, 14, 80, 43}};
  for(unsigned i=0;i<typeRegister->getNofDescriptionUints();++i)
    if(typeRegister->getDescription(i)!=finalDesc[i])
      std::cerr<<"final description vector is different!"<<std::endl;
  
  ___;
  delete typeRegister;
  ___;
}

