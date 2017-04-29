#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include"interpret.h"
#include"stdFunctions.h"

std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;

void whileTest(){
  /*
   * unsigned i=1;
   * unsigned k=1;
   * while(i<10){
   *  k*=i;
   *  i++;
   * }
   */
  auto fi    = std::make_shared<ge::core::Nullary>((unsigned)1u,typeRegister);
  auto fk    = std::make_shared<ge::core::Nullary>((unsigned)1u,typeRegister);
  auto fiend = std::make_shared<ge::core::Nullary>((unsigned)4u,typeRegister);

  auto cond=std::make_shared<ge::core::Less<unsigned>>(typeRegister->sharedAccessor("bool"));
  cond->setInput(0,fi   );
  cond->setInput(1,fiend);

  auto mult=std::make_shared<ge::core::Muls<unsigned>>();
  mult->setInput(0,fk);
  mult->setInput(1,fi);
  
  auto inc=std::make_shared<ge::core::IncrPost<unsigned>>();
  inc->setInput(0,fi);

  auto body=std::make_shared<ge::core::Body>();
  body->addStatement(mult);
  body->addStatement(inc );

  auto wh=std::make_shared<ge::core::While>(cond,body);
  (*wh)();
  std::cout<<(unsigned)(*fk->getOutput())<<std::endl;
}

void ifTest(){
  /*
   * unsigned a=10;
   * unsigned b=12;
   * unsigned c=0;
   * if(a<b)
   *  c=a;
   * else
   *  c=b;
   */
  auto fa=std::make_shared<ge::core::Nullary>((unsigned)10,typeRegister);
  auto fb=std::make_shared<ge::core::Nullary>((unsigned)12,typeRegister);
  auto fc=std::make_shared<ge::core::Nullary>((unsigned)0 ,typeRegister);

  auto cond=std::make_shared<ge::core::Less<unsigned>>(typeRegister->sharedAccessor("bool"));
  cond->setInput(0,fa);
  cond->setInput(1,fb);

  auto trueBody = std::make_shared<ge::core::Ass<unsigned>>();
  trueBody->setInput(0,fc);
  trueBody->setInput(1,fa);

  auto falseBody = std::make_shared<ge::core::Ass<unsigned>>();
  falseBody->setInput(0,fc);
  falseBody->setInput(1,fb);

  auto iff=std::make_shared<ge::core::If>(cond,trueBody,falseBody);

  (*iff)();
  std::cout<<(unsigned)(*fc->getOutput())<<std::endl;
}

int main(){
  typeRegister = std::make_shared<ge::core::TypeRegister>();

  auto va=typeRegister->sharedAccessor<float>(101.f  );
  auto vb=typeRegister->sharedAccessor<float>(1.1f   );
  auto vc=typeRegister->sharedAccessor<float>(2.2f   );
  auto vd=typeRegister->sharedAccessor<float>(1000.f );

  auto fa=std::make_shared<ge::core::Nullary>(va);
  auto fb=std::make_shared<ge::core::Nullary>(vb);
  auto fc=std::make_shared<ge::core::Nullary>(vc);
  auto fd=std::make_shared<ge::core::Nullary>(vd);

  auto f0=std::make_shared<ge::core::Add<float>>(typeRegister->sharedAccessor("f32"));
  f0->setInput(0,fa);
  f0->setInput(1,fb);
  auto f1=std::make_shared<ge::core::Add<float>>(typeRegister->sharedAccessor("f32"));
  f1->setInput(0,f0);
  f1->setInput(1,fc);
  auto f2=std::make_shared<ge::core::Sub<float>>(typeRegister->sharedAccessor("f32"));
  f2->setInput(0,f0);
  f2->setInput(1,fd);
  auto f3=std::make_shared<ge::core::Cast<float,int>>(typeRegister->sharedAccessor("i32"));
  f3->setInput(0,f2);
  (*f1)();
  (*f2)();
  (*f3)();
  std::cout<<(float)(*f1->getOutput())<<std::endl;
  std::cout<<(float)(*f2->getOutput())<<std::endl;
  std::cout<<(int  )(*f3->getOutput())<<std::endl;

  whileTest();
  ifTest();

  return 0;
}
