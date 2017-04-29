#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
//#include<utility>
#include<geCore/dtemplates.h>
#include<geCore/functionRegister.h>
#include<geCore/function.h>
#include<geCore/stdFunctions.h>

template<typename TYPE>std::string name();
template<typename...ARGS>std::string names();
template<typename HEAD,typename...ARGS>std::string names_helper(){return name<HEAD>()+((sizeof...(ARGS)>0)?",":"")+names<ARGS...>();}
template<typename...ARGS>std::string names(){return names_helper<ARGS...>();}
template<>std::string names(){return std::string("");}
template<template<typename...>class CLASS>std::string className();
template<template<typename...>class CLASS,typename... ARGS>std::string name(){return className<CLASS>()+"<"+names<ARGS...>()+">";}
template<>std::string name<void       >(){return "void"    ;}
template<>std::string name<bool       >(){return "bool"    ;}
template<>std::string name<int8_t     >(){return "int8_t"  ;}
template<>std::string name<int16_t    >(){return "int16_t" ;}
template<>std::string name<int32_t    >(){return "int32_t" ;}
template<>std::string name<int64_t    >(){return "int64_t" ;}
template<>std::string name<uint8_t    >(){return "uint8_t" ;}
template<>std::string name<uint16_t   >(){return "uint16_t";}
template<>std::string name<uint32_t   >(){return "uint32_t";}
template<>std::string name<uint64_t   >(){return "uint64_t";}
template<>std::string name<float      >(){return "float"   ;}
template<>std::string name<double     >(){return "double"  ;}
template<>std::string name<std::string>(){return "string"  ;}

namespace jo{
  template <class T, T... I> struct integer_sequence
  {
    template <T N> using append = integer_sequence<T, I..., N>;
    static std::size_t size() { return sizeof...(I); }
    using next = append<sizeof...(I)>;
    using type = T;
  };

  template <class T, T Index, std::size_t N>
    struct sequence_generator
    {
      using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
    };

  template <class T, T Index>
    struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

  template <std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;

  template <class T, T N>
    struct make_integer_sequence
    {
           typedef typename sequence_generator<T, N, N>::type type;
    };
    //using make_integer_sequence = typename sequence_generator<T, N, N>::type;

  template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

  template<class... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;


}

template<typename...TYPES,std::size_t...I>
void printVoids(std::vector<void*>voids,jo::index_sequence<I...>){
  auto ss={((void)(std::cout<<*(TYPES*)voids[I]<<std::endl),0)...};
  (void)ss;
}


template<typename...TYPES>
void printVoids2(std::vector<void*>voids){
  printVoids<TYPES...>(voids,typename jo::make_index_sequence<sizeof...(TYPES)>::type{});
}

int main(){
  std::vector<void*>aa;
  float a = 23.12f;
  int   b = 11    ;
  float c = 43.32f;

  aa.push_back(&a);
  aa.push_back(&b);
  aa.push_back(&c);
  printVoids<float,int,float>(aa,jo::make_index_sequence<3>::type{});
  printVoids2<float,int,float>(aa);
  return 0;
}
