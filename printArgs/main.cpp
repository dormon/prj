#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>

template<typename...ARGS>
std::string argsToStr(ARGS...);

template<typename F,typename...ARGS>
std::string argsToStr_help(F const&a,ARGS...args){
  std::stringstream ss;
  ss<<a;
  if(sizeof...(args)>0)
    ss<<","<<argsToStr(args...);
  return ss.str();
}

template<typename...ARGS>
std::string argsToStr(ARGS...args){
  return argsToStr_help(args...);
}

template<>std::string argsToStr(){
  return "";
}

int main(){
  std::cout<<argsToStr()<<std::endl;
  std::cout<<argsToStr(32)<<std::endl;
  std::cout<<argsToStr(32,1,"asd")<<std::endl;
  std::cout<<argsToStr(32,1,2,3)<<std::endl;
  std::cout<<argsToStr(32,1,2,3)<<std::endl;
  return 0;
}
