#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<chrono>
#include<algorithm>

#include"fce.h"

int main(){
  size_t const iterations = 2000000000;
  {
    size_t buf=2;
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<iterations;++i)
      buf=addX(buf);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto time = double(elapsed.count());
    std::cout<<"except: "<<time<<" s"<<std::endl;
  }
  {
    size_t buf=2;
    auto start = std::chrono::high_resolution_clock::now();
    try{
      for(size_t i=0;i<iterations;++i)
        buf=addX(buf);
    }catch(char const*e){
      std::cerr<<e<<std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto time = double(elapsed.count());
    std::cout<<"except+try,catch: "<<time<<" s"<<std::endl;
  }
  {
    size_t buf=2;
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<iterations;++i)
      buf=add(buf);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto time = double(elapsed.count());
    std::cout<<"no except: "<<time<<" s"<<std::endl;
  }
  {
    size_t buf=2;
    auto start = std::chrono::high_resolution_clock::now();
    try{
      for(size_t i=0;i<iterations;++i)
        buf=add(buf);
    }catch(char const*e){
      std::cerr<<e<<std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto time = double(elapsed.count());
    std::cout<<"no except+try,catch: "<<time<<" s"<<std::endl;
  }


  return 0;
}
