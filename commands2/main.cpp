#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<functional>
#include<utility>
#include<glm/glm.hpp>
#include<geCore/Text.h>
#include<geGL/geGL.h>

class Command{
  public:
    virtual ~Command(){}
    virtual void operator()()=0;
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

/*
template<typename OUTPUT,typename CLASS,typename...ARGS,size_t...I,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
void uber_call(OUTPUT(CLASS::*fce)(ARGS...)const,CLASS const*cls,std::tuple<ARGS...>const&args,std::index_sequence<I...>){
  (cls->*fce)(std::get<I...>(args));
}

template<typename OUTPUT,typename CLASS,typename...ARGS,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>createCommand(OUTPUT(CLASS::*fce)(ARGS...)const,CLASS const*cls,ARGS...args){
  class Function: public Command{
    using IMPL = OUTPUT(CLASS::*)(ARGS...)const;
    public:
      IMPL _impl;
      CLASS const*_cls;
      std::tuple<ARGS...>_args;
      Function(IMPL fce,CLASS const*cls,ARGS...args):_impl(fce),_cls(cls),_args(args...){
      }
      virtual void operator()(){
        assert(this);
        uber_call(this->_impl,this->_cls,this->_args,std::make_index_sequence<sizeof...(ARGS)>{});
      }
  };
  return std::make_shared<Function>(fce,cls,args...);
}
template<typename OUTPUT,typename CLASS>
void uber_call(OUTPUT(CLASS::*fce)()const,CLASS const*cls){
  (cls->*fce)();
}

template<typename OUTPUT,typename CLASS,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>createCommand(OUTPUT(CLASS::*fce)()const,CLASS const*cls){
  class Function: public Command{
    using IMPL = OUTPUT(CLASS::*)()const;
    public:
      IMPL _impl;
      CLASS const*_cls;
      Function(IMPL fce,CLASS const*cls):_impl(fce),_cls(cls){
      }
      virtual void operator()(){
        assert(this);
        uber_call(this->_impl,this->_cls);
      }
  };
  return std::make_shared<Function>(fce,cls);
}
*/

/*
template<
  typename   RETURN_TYPE           ,
  typename   CLASS_TYPE            ,
  typename...FUNCTION_ARGUMENT_TYPE,
  typename...ARGUMENT_TYPE         ,
  size_t  ...ARGUMENT_INDICES      ,
  typename std::enable_if< std::is_same<RETURN_TYPE,void>::value,unsigned>::type = 0>
void callMemberFunctionPointerWithArgumentsAsTuple(
    RETURN_TYPE(CLASS_TYPE::*memberFunctionPointer)(FUNCTION_ARGUMENT_TYPE...),
    CLASS_TYPE*classInstance                                                  ,
    std::tuple<ARGUMENT_TYPE...>const&arguments                               ,
    std::index_sequence<ARGUMENT_INDICES...>                                  ){
  (classInstance->*memberFunctionPointer)(std::get<ARGUMENT_INDICES>(arguments)...);
}
*/
template<
  typename   RETURN_TYPE           ,
  typename   CLASS_TYPE            ,
  typename...FUNCTION_ARGUMENT_TYPE,
  typename...ARGUMENT_TYPE         ,
  size_t  ...ARGUMENT_INDICES      ,
  typename std::enable_if<std::is_same<RETURN_TYPE,void>::value && sizeof...(FUNCTION_ARGUMENT_TYPE)==sizeof...(ARGUMENT_TYPE),unsigned>::type = 0>
std::shared_ptr<Command>createCommand(
    RETURN_TYPE(CLASS_TYPE::*memberFunctionPointer)(FUNCTION_ARGUMENT_TYPE...),
    CLASS_TYPE*classInstance,
    ARGUMENT_TYPE...arguments){

  using FunctionType = RETURN_TYPE(CLASS_TYPE::*)(FUNCTION_ARGUMENT_TYPE...);

  class Function: public Command{
    public:
      FunctionType                 _memberFunctionPointer;
      CLASS_TYPE*                  _classInstance;
      std::tuple<ARGUMENT_TYPE...> _arguments;
      Function(
          FunctionType  const&     f,
          CLASS_TYPE*   const&     c,
          ARGUMENT_TYPE const& ... v):
        _memberFunctionPointer(f   ),
        _classInstance        (c   ),
        _arguments            (v...){}
      virtual void operator()(){
        assert(this);
        //auto seq = std::make_index_sequence<sizeof...(ARGUMENT_TYPE)>{};
        (this->_classInstance->*this->_memberFunctionPointer)(std::get<ARGUMENT_INDICES>(this->_arguments)...);
        /*
        callMemberFunctionPointerWithArgumentsAsTuple(
            this->_memberFunctionPointer                        ,
            this->_classInstance                                ,
            this->_arguments                                    ,
            std::make_index_sequence<sizeof...(ARGUMENT_TYPE)>{});*/
      }
  };
  return std::make_shared<Function>(
      memberFunctionPointer,
      classInstance        ,
      arguments...         );
}

// */

/*
template<typename OUTPUT,typename...ARGS,size_t...I,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
void uber_call(OUTPUT(*fce)(ARGS...),std::tuple<ARGS...>const&args,std::index_sequence<I...>){
  fce(std::get<I...>(args));
}

template<typename OUTPUT,typename...ARGS,typename std::enable_if< std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Command>createCommand(OUTPUT(*fce)(ARGS...),ARGS...args){
  using IMPL = OUTPUT(*)(ARGS...);
  class Function: public Command{
    public:
      IMPL _impl;
      std::tuple<ARGS...>_args;
      Function(IMPL fce,ARGS...args):_impl(fce),_args(args...){
      }
      virtual void operator()(){
        assert(this);
        uber_call(this->_impl,this->_args,std::make_index_sequence<sizeof...(ARGS)>{});
      }
  };
  return std::make_shared<Function>(fce,args...);
}

void add(int a,int b){
  std::cout<<a+b<<std::endl;
}
*/

template<typename CLASS,typename...ARGS,typename...VARGS>
void call(void(CLASS::*fce)(ARGS...),CLASS*cls,VARGS...args){
  (cls->*fce)(args...);
}

template<typename CLASS,typename...ARGS>
void call(void(CLASS::*fce)(ARGS...),CLASS*cls,ARGS...args){
  (cls->*fce)(args...);
}


int main(int,char*[]){
  /*
  ge::gl::Context*ctx = nullptr;
  auto cmd2 = createCommand(&ge::gl::Context::glBindVertexArray,ctx,0u);
  (*cmd2)();

  auto vbo = std::make_shared<ge::gl::Buffer>(100);
  auto cmd3 = createCommand(&ge::gl::Buffer::bind,&*vbo,GLenum(GL_ELEMENT_ARRAY_BUFFER));
  (*cmd3)();

  auto vao = std::make_shared<ge::gl::VertexArray>();
  auto cmd4 = createCommand(&ge::gl::VertexArray::bind,&*vao);
  (*cmd4)();
  */
  
  ge::gl::Program*prg = nullptr;
  //call(&ge::gl::Program::set4fv,prg,"position",nullptr,1);
  auto cmd5 = createCommand(&ge::gl::Program::set4fv,prg,"position",nullptr,1);
  (*cmd5)();


  return 0;
}
