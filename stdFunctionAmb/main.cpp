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

void call(std::function<void(float)>const&f){
  f(10.f);
}

void call(std::function<void()>const&f){
  f();
}

void simple(std::string const&n){
  std::cout << "simple("<<n<<")" << std::endl;
}

void simple2(float a){
  std::cout << "simple2("<<a<<")" << std::endl;
}

int main(){
  call(std::bind(simple,"ahoj"));
  return 0;
}





