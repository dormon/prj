#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

template<typename R,typename...ARGS>
class Function{
  public:
    using Type = R(ARGS...);
    using InnerType = void();
    Function(Type*f){
      impl = reinterpret_cast<InnerType*>(f);
    }
    R operator()(ARGS const&...args){
      return reinterpret_cast<Type*>(impl)(args...);
    }
  protected:
    InnerType*impl;
};

//template<typename T,typename R,typename...ARGS>
//Function<R,ARGS...>bind(Function<R,T,ARGS...>const&f,T const&t){
//
//}

float add(float a,float b){
  return a+b;
}

int main(){
  Function<float,float,float> f(add);
  std::cout << f(10,30) << std::endl;
  return 0;
}





