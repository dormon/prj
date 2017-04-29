#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<glm/glm.hpp>
#include<functional>
#include<geCore/Text.h>
#include<geGL/geGL.h>

class Command{
  public:
    virtual ~Command(){}
    virtual void operator()()=0;
};

class AtomicCommand: public Command, public std::function<void()>{
  public:
    AtomicCommand(std::function<void()>const&fce):std::function<void()>(fce){}
    virtual ~AtomicCommand()override{}
    virtual void operator()()override{
      assert(this);
      this->std::function<void()>::operator()();
    }
};

class CommandList: public Command, public std::vector<std::shared_ptr<Command>>{
  public:
    virtual ~CommandList(){}
    virtual void operator()()override{
      assert(this);
      for(auto const&x:*this)
        (*x)();
    }
};

template<typename OUTPUT,typename CLASS,typename...ARGS,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>createCommand(OUTPUT(CLASS::*fce)(ARGS...)const,CLASS*cls,ARGS...args){
  return std::make_shared<AtomicCommand>(std::bind(fce,cls,args...));
}

int main(int,char*[]){
  auto cmd = std::make_shared<AtomicCommand>([](){std::cout<<"a"<<std::endl;});
  ge::gl::Context*ctx = nullptr;
  auto cmd2 = createCommand(&ge::gl::Context::glBindVertexArray,ctx,0u);

  //auto cmd2 = std::make_shared<AtomicCommand>(std::bind(&ge::gl::Context::glBindVertexArray,nullptr,0));
  (*cmd)();

  return 0;
}
