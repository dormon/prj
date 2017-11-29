#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>

#include"FunctionArgumentType.h"

class Test{
  public:
    bool ahoj(float a,int b)const{
      return a>static_cast<float>(b);
    };
    bool neho(float const&a){
      return a==0.f;
    }
};

float fce(float a){return a*a;}

int main(){
  FunctionReturnType<decltype(fce)>::type a;
  MemberFunctionReturnType<decltype(&Test::ahoj)>::type b;
  MemberFunctionReturnType<decltype(&Test::neho)>::type c;
  a = 1.3f;
  b = false;
  c = true;
  b = b && c;

  if(b)
    std::cout << a << std::endl;

  return 0;
}
