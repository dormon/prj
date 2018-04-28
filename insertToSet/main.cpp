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

template<typename,typename>
struct Insert;

template<typename A,typename...B>
struct Insert<A,std::tuple<B...>>{
  using type = std::conditional_t<contain<A,B...>(),std::tuple<B...>,std::tuple<B...,A>>;
};


int main() { 
  static_assert(contain<float,int,char,float,double>());
  static_assert(!contain<float>());
  static_assert(!contain<float,int>());
  static_assert(std::is_same_v<Insert<float,std::tuple<int>>::type,std::tuple<int,float>>);
  static_assert(std::is_same_v<Insert<float,std::tuple<float,int>>::type,std::tuple<float,int>>);
  return 0; 
}
