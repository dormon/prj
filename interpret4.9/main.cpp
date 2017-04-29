#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>


int main(){
  auto typeRegister = std::make_shared<ge::core::TypeRegister>();
  auto fa=std::make_shared<ge::core::Nullary>(typeRegister,101.f );
  auto fb=std::make_shared<ge::core::Nullary>(typeRegister,1.1f  );
  auto fc=std::make_shared<ge::core::Nullary>(typeRegister,2.2f  );
  auto fd=std::make_shared<ge::core::Nullary>(typeRegister,1000.f);

  auto f0=std::make_shared<ge::core::Add<float>>(typeRegister,typeRegister->sharedAccessor("f32"));
  f0->bindInput(0,fa);
  f0->bindInput(1,fb);
  auto f1=std::make_shared<ge::core::Add<float>>(typeRegister,typeRegister->sharedAccessor("f32"));
  f1->bindInput(0,f0);
  f1->bindInput(1,fc);
  auto f2=std::make_shared<ge::core::Sub<float>>(typeRegister,typeRegister->sharedAccessor("f32"));
  f2->bindInput(0,f0);
  f2->bindInput(1,fd);
  auto f3=std::make_shared<ge::core::Cast<float,int>>(typeRegister,typeRegister->sharedAccessor("i32"));
  f3->bindInput(0,f2);

  (*f3)();

  std::cout<<(int)(*f3->getOutput())<<" == "<<(int)((101.f+1.1f)-1000.f)<<std::endl;

  return 0;
}





