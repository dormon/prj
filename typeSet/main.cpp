#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>

template<typename...>
struct TypeSet{};

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
struct Insert<A,TypeSet<B...>>{
  using type = std::conditional_t<contain<A,B...>(),TypeSet<B...>,TypeSet<B...,A>>;
};

template<typename A,typename>
struct Union{
  using type = A;
};

template<typename A,typename B,typename...C>
struct Union<A,TypeSet<B,C...>>{
  using type = typename Union<typename Insert<B,A>::type,TypeSet<C...>>::type;
};


int main() { 
  static_assert(contain<float,int,char,float,double>());
  static_assert(!contain<float>());
  static_assert(!contain<float,int>());
  static_assert(std::is_same_v<Insert<float,TypeSet<int>>::type,TypeSet<int,float>>);
  static_assert(std::is_same_v<Insert<float,TypeSet<float,int>>::type,TypeSet<float,int>>);
  static_assert(std::is_same_v<Union<TypeSet<float>,TypeSet<double>>::type,TypeSet<float,double>>);
  static_assert(std::is_same_v<Union<TypeSet<float,int>,TypeSet<double>>::type,TypeSet<float,int,double>>);
  static_assert(std::is_same_v<Union<TypeSet<float>,TypeSet<double,char>>::type,TypeSet<float,double,char>>);
  //static_assert(std::is_same_v<Union<TypeSet<float,int,char>,TypeSet<float,std::string,double>>::type,TypeSet<float,int,char,std::string,double>>);
  return 0; 
}