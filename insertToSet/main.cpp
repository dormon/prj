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

template<typename...E>
constexpr size_t size(TypeSet<E...>){
  return sizeof...(E);
}

template<typename A>
constexpr size_t size(){
  return size(A{});
}

template<typename>
constexpr bool contain(TypeSet<>){
  return false;
}

template<typename A,typename B,typename...E>
constexpr bool contain(TypeSet<B,E...>){
  if constexpr(std::is_same_v<A,B>)
    return true;
  return contain<A>(TypeSet<E...>{});
}

template<typename A,typename S>
constexpr bool contain(){
  return contain<A>(S{});
}

template<typename>
constexpr bool subset(TypeSet<>){
  return true;
}

template<typename B,typename AH,typename...AT>
constexpr bool subset(TypeSet<AH,AT...>){
  return contain<AH,B>() && subset<B>(TypeSet<AT...>{});
}

template<typename A,typename B>
constexpr bool subset(){
  return subset<B>(A{});
}

template<typename A,typename B>
constexpr bool equal(){
  return subset<A,B>() && subset<B,A>();
}

template<typename,typename>
struct InsertT{};

template<typename A,typename...B>
struct InsertT<A,TypeSet<B...>>{
  using type = std::conditional_t<contain<A,TypeSet<B...>>(),TypeSet<B...>,TypeSet<B...,A>>;
};

template<typename...A>
using Insert = typename InsertT<A...>::type;



template<typename,typename>
struct Remove{};

template<typename A,typename>
struct UnionT{
  using type = A;
};

template<typename A,typename B,typename...C>
struct UnionT<A,TypeSet<B,C...>>{
  using type = typename UnionT<Insert<B,A>,TypeSet<C...>>::type;
};

template<typename...A>
using Union = typename UnionT<A...>::type;


template<typename A,typename>
struct IntersectT{
  using type = TypeSet<>;
};

template<typename A,typename B,typename...C>
struct IntersectT<A,TypeSet<B,C...>>{
  using type = typename std::conditional_t<contain<B,A>(),Union<TypeSet<B>,typename IntersectT<A,TypeSet<C...>>::type>,typename IntersectT<A,TypeSet<C...>>::type>;
};

template<typename...A>
using Intersect = typename IntersectT<A...>::type;

int main() {
  static_assert( size<TypeSet<         >>() == 0);
  static_assert( size<TypeSet<float    >>() == 1);
  static_assert( size<TypeSet<float,int>>() == 2);
  static_assert(!contain<float                  ,TypeSet<              >>());
  static_assert( contain<float                  ,TypeSet<float         >>());
  static_assert(!contain<float                  ,TypeSet<int,char      >>());
  static_assert( contain<float                  ,TypeSet<int,char,float>>());
  static_assert( contain<float                  ,TypeSet<int,float,char>>());
  static_assert( contain<float                  ,TypeSet<float,char,int>>());
  static_assert( subset <TypeSet<              >,TypeSet<              >>());
  static_assert( subset <TypeSet<              >,TypeSet<float         >>());
  static_assert( subset <TypeSet<float         >,TypeSet<float         >>());
  static_assert( subset <TypeSet<float         >,TypeSet<float,int     >>());
  static_assert( subset <TypeSet<float,int     >,TypeSet<float,int     >>());
  static_assert( subset <TypeSet<float,int,char>,TypeSet<float,int,char>>());
  static_assert(!subset <TypeSet<float,int,char>,TypeSet<float,int     >>());
  static_assert(!subset <TypeSet<float         >,TypeSet<              >>());
  static_assert( equal  <TypeSet<              >,TypeSet<              >>());
  static_assert(!equal  <TypeSet<              >,TypeSet<float         >>());
  static_assert(!equal  <TypeSet<float         >,TypeSet<              >>());
  static_assert(!equal  <TypeSet<float,int     >,TypeSet<              >>());
  static_assert( equal  <TypeSet<float         >,TypeSet<float         >>());
  static_assert( equal  <TypeSet<float,int     >,TypeSet<float,int     >>());
  static_assert(!equal  <TypeSet<              >,TypeSet<float,int     >>());
  static_assert(!equal  <TypeSet<float         >,TypeSet<float,int     >>());
  static_assert( equal  <TypeSet<int,float     >,TypeSet<float,int     >>());
  static_assert(std::is_same_v<Insert   <float                  ,TypeSet<                         >>,TypeSet<float                >>);
  static_assert(std::is_same_v<Insert   <float                  ,TypeSet<int                      >>,TypeSet<int,float            >>);
  static_assert(std::is_same_v<Insert   <float                  ,TypeSet<float,int                >>,TypeSet<float,int            >>);
  static_assert(std::is_same_v<Insert   <int                    ,TypeSet<float,int                >>,TypeSet<float,int            >>);
  static_assert(std::is_same_v<Union    <TypeSet<              >,TypeSet<                         >>,TypeSet<                     >>);
  static_assert(std::is_same_v<Union    <TypeSet<              >,TypeSet<int                      >>,TypeSet<int                  >>);
  static_assert(std::is_same_v<Union    <TypeSet<float         >,TypeSet<                         >>,TypeSet<float                >>);
  static_assert(std::is_same_v<Union    <TypeSet<              >,TypeSet<int,float                >>,TypeSet<int,float            >>);
  static_assert(std::is_same_v<Union    <TypeSet<float         >,TypeSet<int                      >>,TypeSet<float,int            >>);
  static_assert(std::is_same_v<Union    <TypeSet<float,double  >,TypeSet<                         >>,TypeSet<float,double         >>);
  static_assert(std::is_same_v<Union    <TypeSet<float,double  >,TypeSet<int,char                 >>,TypeSet<float,double,int,char>>);
  static_assert(std::is_same_v<Intersect<TypeSet<              >,TypeSet<                         >>,TypeSet<                     >>);
  static_assert(std::is_same_v<Intersect<TypeSet<float         >,TypeSet<                         >>,TypeSet<                     >>);
  static_assert(std::is_same_v<Intersect<TypeSet<              >,TypeSet<float                    >>,TypeSet<                     >>);
  static_assert(std::is_same_v<Intersect<TypeSet<float         >,TypeSet<float                    >>,TypeSet<float                >>);
  static_assert(std::is_same_v<Intersect<TypeSet<float,int     >,TypeSet<float                    >>,TypeSet<float                >>);
  static_assert(std::is_same_v<Intersect<TypeSet<float,int     >,TypeSet<float,int                >>,TypeSet<float,int            >>);
  static_assert(std::is_same_v<Intersect<TypeSet<float,int,char>,TypeSet<double,int16_t,char,float>>,TypeSet<char,float           >>);
  return 0; 
}
