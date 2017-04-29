#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>

template<typename...ARGS,typename = std::enable_if<sizeof...(ARGS)==0>>
void printArgs2(ARGS...){
}

template<typename HEAD,typename...TAIL>
void printArgs2(HEAD head,TAIL...tail){
  std::cerr<<","<<head;
  printArgs2(tail...);
}

template<typename...ARGS,typename = std::enable_if<sizeof...(ARGS)==0>>
void printArgs(){
}

template<typename HEAD,typename...TAIL>
void printArgs(HEAD head,TAIL...tail){
  std::cerr<<head;
  printArgs2(tail...);
}

template<typename...ARGS,typename = std::enable_if<sizeof...(ARGS)!=0>>
void printError(std::string fceName,std::string message,ARGS...args){
  std::cerr<<"ERROR: "<<fceName<<"(";
  printArgs(args...);
  std::cerr<<") - "<<message;
}

int main(){
  printError("getFce","no such function");
  return 0;
}
