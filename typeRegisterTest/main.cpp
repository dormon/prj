#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geCore/TypeRegister.h>
#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>
#include"unitTests.h"

class WRT{
  public:
    int*data = nullptr;
    WRT(int size){
      std::cout<<"volam constructor"<<std::endl;
      this->data=new int[size];
    }
    ~WRT(){
      std::cout<<"volam destructor"<<std::endl;
      delete[]data;
    }
};

void alloc(WRT**p){
  *p=new WRT(100);
}

class Sin: public ge::core::Function{
  public:
    Sin():Function(1){
      this->_setInput(0,ge::core::TypeRegister::getTypeTypeId<float>());
      this->_setOutput(ge::core::TypeRegister::getTypeTypeId<float>());
    }
    void operator()(){
      std::cout<<"Sin je volan"<<std::endl;
    }
};

class Shader{
  protected:
    int*_data;
  public:
    Shader(unsigned n){
      std::cerr<<"Shader constructor"<<std::endl;
      this->_data = new int[n];
    }
    ~Shader(){
      std::cerr<<"Shader destructor"<<std::endl;
      delete[]this->_data;
    }
};

int main(int argc,char*argv[]){
  std::cout<<ge::core::TypeRegister::I64<<std::endl;

  //registrationTest();
  //return 0;

  std::shared_ptr<ge::core::TypeRegister>typeRegister=std::make_shared<ge::core::TypeRegister>();
  typeRegister->addType("mujTyp",ge::core::TypeRegister::ARRAY,2,ge::core::TypeRegister::I32);
  typeRegister->addType("mujTyp2",ge::core::TypeRegister::STRUCT,2,"mujTyp","f32");
  ge::core::Accessor dat=typeRegister->allocAccessor("mujTyp");
  dat[0]=12;
  dat[1]=32;
  std::cout<<(unsigned)dat[0]<<std::endl;
  std::cout<<dat.data2Str()<<std::endl;
  ge::core::Accessor dat2=typeRegister->allocAccessor("mujTyp2");
  dat2[0][0]=32;
  dat2[0][1]=10;
  dat2[1]=13.3f;
  std::cout<<dat2.data2Str()<<std::endl;
  int a=(int)dat[0];

  typeRegister->addType("func",ge::core::TypeRegister::FCE,"f32",1,"f32");
  auto ff=typeRegister->sharedAccessor("func");
  auto fsin=std::make_shared<Sin>();
  //(*fsin)();
  (std::shared_ptr<ge::core::Function>&)(*ff)=fsin;//TODO otestovat typy
  (*((std::shared_ptr<ge::core::Function>&)(*ff)))();
  std::cout<<typeRegister->toStr(ff->getId())<<std::endl;
  typeRegister->addClassCD<std::shared_ptr<Shader>>("SharedShader");
  auto sshader = typeRegister->sharedAccessor("SharedShader");
  ___;
  (std::shared_ptr<Shader>&)*sshader = std::make_shared<Shader>(100);
  (std::shared_ptr<Shader>&)*sshader = nullptr;
  ___;
  //*sshader = nullptr;
  

  return 0;

  //ge::core::TypeRegister*typeRegister=new ge::core::TypeRegister();
  /*
  typeRegister->addType("bb",ge::core::TypeRegister::ARRAY,3,2000);
  typeRegister->addType("bb",ge::core::TypeRegister::ARRAY,3);
  typeRegister->addType("bb",ge::core::TypeRegister::ARRAY);
  typeRegister->addType("bb",ge::core::TypeRegister::STRUCT,2,"f32","i32");
  typeRegister->addType("bb",ge::core::TypeRegister::STRUCT,2,"f32","");
  typeRegister->addType("bb",ge::core::TypeRegister::STRUCT,2,"f32");
  typeRegister->addType("bb",ge::core::TypeRegister::STRUCT,2);
  typeRegister->addType("bb",ge::core::TypeRegister::STRUCT);
  typeRegister->addType("bb",ge::core::TypeRegister::ARRAY,2,ge::core::TypeRegister::ARRAY,2,"i32");

  */
  typeRegister->addClassD<WRT>("WRT");

  std::shared_ptr<ge::core::Accessor>ptr=typeRegister->sharedAccessor<WRT>("WRT",10);
  std::cerr<<typeRegister->toStr()<<std::endl;

  std::shared_ptr<ge::core::Accessor>ewrt=typeRegister->sharedEmptyAccessor("WRT",[](unsigned char*data){delete (WRT*)data;});
  alloc(*ewrt);
  //std::cout<<((WRT&)*ewrt).data<<std::endl;
  //ptr.~shared_ptr();
  //typeRegister.~shared_ptr();
  //ptr.~shared_ptr();
  

  ge::core::Accessor udata=typeRegister->allocAccessor("u32");
  udata=32u;

  //ge::core::Accessor val(13.f,typeRegister);

  ge::core::Accessor v3=typeRegister->allocAccessor("vec3");
  v3[0]=32.2f;
  v3[1]=12.1f;
  v3[2]=0.3f;

  typeRegister->addType("mat4",ge::core::TypeRegister::ARRAY,4,"f32");

  ge::core::Accessor m4=typeRegister->allocAccessor("mat4");
  m4[0][2]=123.32f;


  ge::core::Accessor v4=typeRegister->allocAccessor("vec4");
  float*uk=v4;
  float*uk1=v4[2];
  uk[0]=13.3;
  uk[1]=7.1;
  *uk1=9.3;
  std::cout<<(float)v4[0]<<std::endl;
  std::cout<<(float)v4[1]<<std::endl;
  std::cout<<(float)v4[2]<<std::endl;

  return 0;
}
