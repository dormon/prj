#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>
#include <cmath>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

struct Result2{
  static const uint32_t success  = 0u;
  static const uint32_t notReady = 1u;
  struct A{
    static const uint32_t set   = 2u;
    static const uint32_t reset = 3u;
  }static const event;
  struct B{
    struct A{
      static const uint32_t host_memory   = 4u;
      static const uint32_t device_memory = 4u;
    }static const out_of;
    static const uint32_t initialization_failed = 6u;
    static const uint32_t device_lost           = 7u;
  }static const error;
  uint32_t value = success;
};

struct Result{
  uint32_t v;
  Result(uint32_t v):v(v){}
  bool operator==(Result const&o)const{return v==o.v;}
};

namespace result{
  auto const success                 = Result(0u);
  auto const notReady                = Result(1u);
  namespace event{
    auto const set                   = Result(2u);
    auto const reset                 = Result(3u);
  };
  namespace error{
    namespace out_of{
      auto const host_memory         = Result(4u);
      auto const device_memory       = Result(5u);
    };
    auto const initialization_failed = Result(6u);
    auto const device_lost           = Result(7u);
  };
};

std::string str(Result const&t){
  if(t==result::success)return "VK_SUCCESS";
  return "";
}

std::ostream& operator<<(std::ostream& os, Result const& obj){
  os << obj.v;
  return os;
}

void a(){
  std::cerr << result::event::set << std::endl;
}

int main(){
  Result2::error.out_of.host_memory;
  return 0;
}


