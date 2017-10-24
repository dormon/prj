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
#include</media/windata/builtLibs/linux/installedGLM/include/glm/glm.hpp>
//#include<geCore/Text.h>
//#include<geGL/geGL.h>



template<
  typename TO  ,
  typename FROM,
  typename std::enable_if<std::is_convertible<FROM,TO>::value,unsigned>::type = 0>
TO convert(FROM const&from){
  return from;
}

template<typename TO>
TO convert(TO(*functionPointer)()){
  return functionPointer();
}

template<typename TO>
TO convert(std::function<TO()>const&functionPointer){
  return functionPointer();
}

template<
  typename TO  ,
  typename FROM,
  typename std::enable_if<std::is_convertible<FROM,TO>::value,unsigned>::type = 0>
TO convert(std::shared_ptr<FROM> const&from){
  return *from;
}

template<
  typename...TO  ,
  typename...FROM,
  size_t  ...I   >
std::tuple<TO...>convert(std::tuple<FROM...>const&from,std::index_sequence<I...>){
  return std::tuple<TO...>(convert<TO>(std::get<I>(from))...);
}

template<
  typename...TO  ,
  typename...FROM,
  typename std::enable_if<sizeof...(TO)==sizeof...(FROM),unsigned>::type = 0>
std::tuple<TO...>convert(std::tuple<FROM...>const&from){
  return convert<TO...>(from,std::make_index_sequence<sizeof...(FROM)>{});
}

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

template<
  typename   RETURN_TYPE           ,
  typename...FUNCTION_ARGUMENT_TYPE,
  typename...ARGUMENT_TYPE         ,
  size_t  ...ARGUMENT_INDICES      ,
  typename std::enable_if< std::is_same<RETURN_TYPE,void>::value,unsigned>::type = 0>
void callFunctionPointerWithArgumentsAsTuple(
    RETURN_TYPE(*functionPointer)(FUNCTION_ARGUMENT_TYPE...),
    std::tuple<ARGUMENT_TYPE...>const&arguments             ,
    std::index_sequence<ARGUMENT_INDICES...>                ){
  functionPointer(std::get<ARGUMENT_INDICES>(convert<FUNCTION_ARGUMENT_TYPE...>(arguments))...);
}

template<
  typename   RETURN_TYPE,
  typename...FUNCTION_ARGUMENT_TYPE,
  typename...ARGUMENT_TYPE,
  typename std::enable_if<
    std::is_same<RETURN_TYPE,void>::value && sizeof...(FUNCTION_ARGUMENT_TYPE)==sizeof...(ARGUMENT_TYPE),
    unsigned>::type = 0>
std::shared_ptr<Command>createCommand(
    RETURN_TYPE(*functionPointer)(FUNCTION_ARGUMENT_TYPE...),
    ARGUMENT_TYPE...arguments){
  class Function: public Command{
    public:
      decltype(functionPointer)   _functionPointer;
      std::tuple<ARGUMENT_TYPE...>_arguments      ;
      Function(
          decltype(functionPointer)f,
          ARGUMENT_TYPE...         v):
        _functionPointer(f   ),
        _arguments      (v...){}
      virtual void operator()(){
        assert(this);
        callFunctionPointerWithArgumentsAsTuple(
            this->_functionPointer                              ,
            this->_arguments                                    ,
            std::make_index_sequence<sizeof...(ARGUMENT_TYPE)>{});
      }
  };
  return std::make_shared<Function>(
      functionPointer,
      arguments...   );
}

void print1(){
  std::cout<<"ahoj svete"<<std::endl;
}

void print2(int a,float b){
  std::cout<<a<<std::endl;
  std::cout<<b<<std::endl;
}

int dejCislo(){
  return 42;
}

float add(float x,float y){
  return x+y;
}

int main(int,char*[]){
  CommandList commands;
  commands.push_back(createCommand(print1));
  commands.push_back(createCommand(print2,1,1.1f));
  commands.push_back(createCommand(print2,1,1.1f));
  commands.push_back(createCommand(print2,std::make_shared<int32_t>(10),1.1f));
  commands.push_back(createCommand(print2,std::make_shared<int8_t>(10),std::make_shared<float>(1)));
  commands.push_back(createCommand(print2,dejCislo,std::make_shared<float>(1)));
  commands.push_back(createCommand(print2,dejCislo,std::bind(add,1.f,2.2f)));

  commands();

  return 0;
}
