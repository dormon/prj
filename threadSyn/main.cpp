#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstring>
#include <functional>
#include <typeinfo>
#include <set>
#include <chrono>
#include <stack>
#include <thread>

uint32_t v = 0;

int main(){
  auto reader = std::thread([&](){
    for(uint32_t i=0;i<1000;++i)
    std::cerr << v << " ";
  });
  auto writer = std::thread([&](){
    for(uint32_t i=0;i<1000;++i)
      v=i;
  });
  reader.join();
  writer.join();
  return 0;
}
