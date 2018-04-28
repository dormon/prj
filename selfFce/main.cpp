#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>

using FCE = void*(*)();



struct F{
  F*me;
  std::unique_ptr<F>a;
  std::unique_ptr<F>operator()(bool topMost = true){
    if(topMost){
      auto f0 = std::move(a);
      while(true){
        if(!f0)break;
        auto f1 = f0->operator()(false);
        a = std::move(f0);
      }
      a = std::move(f0);

      auto f1 = f0->operator()(false);
      if(!f1){
        a = std::move(f0);
        return nullptr;
      }
      std::unique_ptr<F>f2;
      do{
        f2 = f1->operator()(false);
        f0->a = std::move(f1);
      }while(f2);
      return nullptr;
    }
    return std::move(a);
  }
};

int main() {
  return 0;
}
