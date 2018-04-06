#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>

template <char... chars>
using tstring = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr tstring<chars...> operator "" _tstr() { return { }; }

template <typename>
struct X;

template <char... elements>
struct X<tstring<elements...>> {
    const char* GetString() const
    {
        static constexpr char str[sizeof...(elements) + 1] = { elements..., '\0' };
        return str;
    }
};

/*
template<size_t N,size_t... I>
constexpr void fce1(char const(&ar)[N],std::index_sequence<I...>){
  return tstring<ar[I]...>{};
}

template<size_t N,char...chars>
constexpr tstring<chars...> fce0(char const(&ar)[N]){
  return fce1(ar,std::make_index_sequence<N>{});
}


template<size_t N>
constexpr void fce2(char const(&ar)[N]){
  std::cerr << N << std::endl;
}
*/

#define STRINGIFY(S) STRINGIFY2(S)
#define STRINGIFY2(S) #S

template<typename>inline std::string getSource();

#define CAPTURE_STRUCT(CLASS,IMPL)\
IMPL \
template<>inline std::string getSource<CLASS>(){return STRINGIFY(IMPL);}


CAPTURE_STRUCT(Particle,
struct Particle{
  float position;
  float velocity;
  float mass;
};
/*(*/)

struct Empty{};

struct INTGetType{
  using type = int;
};

#define CHARTYPE(c,type0,type1) std::conditional<head==c,type0,type1>::type

template<char head,char...chars>
struct INGetType: std::conditional<head=='t'&&sizeof...(chars)==0,INTGetType,Empty>::type{};

template<char head,char...chars>
struct IGetType: std::conditional<head=='n',INGetType<chars...>,Empty>::type{};

template<char head,char...chars>
struct GetType:  std::conditional<head=='i',IGetType<chars...>,Empty>::type{};


template<char head,typename HEAD_TYPE,typename...TAIL_TYPES>struct TypeList{};

template<                   typename...TAIL_TYPES>struct TypeList<'f',          float,TAIL_TYPES...>{};
template<                   typename...TAIL_TYPES>struct TypeList<'i',          int  ,TAIL_TYPES...>{};
template<typename HEAD_TYPE,typename...TAIL_TYPES>struct TypeList<' ',HEAD_TYPE,int  ,TAIL_TYPES...>{};

template<char head_char,typename HEAD_TYPE,template<typename...TAIL_TYPES>class TYPE_LIST,char...tail_chars>
struct ToTypeList{};

//template<template<typename...TAIL_TYPES>class TYPE_LIST,char...tail_chars>
//struct ToTypeList<'f',float,TYPE_LIST<TAIL_TYPES...>,tail_chars...>{};

template<char CHAR_HEAD,typename HEAD_TYPE,char...CHAR_TAIL,typename...TAIL_TYPES>
void convertCharsToTypes(std::tuple<HEAD_TYPE,TAIL_TYPES...>);

template<char a>
void printChar(){
  std::cout << a << std::endl;
}

template<size_t... Is>
struct seq { };

template<size_t N, size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

template<size_t...ARGS>
typename std::enable_if<sizeof...(ARGS)==0,void>::type printTemplateList(){}

template<size_t HEAD,size_t... TAIL>
void printTemplateList(){
  std::cout << HEAD << std::endl;
  printTemplateList<TAIL...>();
}

template<size_t... ARGS>
void printTemplateList2(seq<ARGS...>){
  printTemplateList<ARGS...>();
}


template<size_t...I>
struct Tokens{};

template<size_t...C>
struct Chars{};

template<size_t...>
struct Tokenize{};





int main(int, char*[]) {
  printChar<80>();
  typename GetType<'i','n','t'>::type asdasd;
  tstring<'a','b','c'>aa{};

  X<decltype("struct Particle{ float position;};"_tstr)>a;
  std::cout <<  a.GetString() << std::endl;

  static_cast<void>(a);
  return EXIT_SUCCESS;
}
