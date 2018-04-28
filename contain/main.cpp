#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>

template<typename>
constexpr bool contain(){
  return false;
}

template<typename A,typename B,typename...C>
constexpr bool contain(){
  if constexpr(std::is_same_v<A,B>)
    return true;
  return contain<A,C...>();
}

int main() { 
  static_assert(contain<float,int,char,float,double>());
  static_assert(!contain<float>());
  static_assert(!contain<float,int>());
  return 0; 
}
