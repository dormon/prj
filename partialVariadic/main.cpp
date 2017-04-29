#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

template<typename OUTPUT,typename...ARGS>
OUTPUT caller(OUTPUT(*fce)(ARGS...),ARGS... args){
  return fce(args...);
}

template<typename OUTPUT,typename...ARGS>
void doer(OUTPUT(*fce)(ARGS...),ARGS...args){
  OUTPUT res = caller(fce,args...);
  std::cout<<res<<std::endl;
}

float add(float a,float b){
  return a+b;
}

void print(int a){
  std::cout<<a<<std::endl;
}

int main(){
  std::cout<<caller(add,1.f,2.f)<<std::endl;
  caller(print,32);
  doer(add,1.f,2.f);
  return 0;
}
