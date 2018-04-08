#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <array>

template<typename T>
constexpr size_t translateTypeToId();

template<>constexpr size_t translateTypeToId<float>(){return 0;}
template<>constexpr size_t translateTypeToId<int>(){return 1;}
template<>constexpr size_t translateTypeToId<char>(){return 2;}

template<size_t N>
void printN(){
  std::cout << N << std::endl;
}

template<typename T>
void printT(){
  printN<translateTypeToId<T>()>();
}

template<char...CHARS>
constexpr std::array<size_t,sizeof...(CHARS)> toToken();

template<int N>
struct A {
    constexpr A() : arr() {
        for (auto i = 0; i != N; ++i)
            arr[i] = i; 
    }
    int arr[N];
};

template<size_t[]>
void print(){}

template<size_t... Is>
struct seq { };

template<size_t N, size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> { };


template<char...I>
struct gen_t{};

template<char...I>
struct a_t{};

template<char...I>
struct a_t<'a',I...>:a_t<I...>{};



template<char...I>
struct gen_t<'i',I...>:a_t<I...>{};

template<size_t...>
struct STATE{};

#define STATE(NAME)\
  template<size_t...I>\
  struct NAME{}

#define TRANSITION(STATE_A,SYMBOL,STATE_B)\
  template<size_t...I>\
  struct STATE_A<SYMBOL,I...>:STATE_B<I...>{}

STATE(START);
STATE(FLOAT);
STATE(INT);
TRANSITION(START,' ',START);
TRANSITION(START,'f',FLOAT);
TRANSITION(START,'i',INT  );
TRANSITION(FLOAT,' ',START);
TRANSITION(FLOAT,'f',FLOAT);
TRANSITION(FLOAT,'i',INT  );
TRANSITION(INT  ,' ',START);
TRANSITION(INT  ,'f',FLOAT);
TRANSITION(INT  ,'i',INT  );

template<size_t...I,typename...T>
constexpr void bla(START<I...>){}

template<size_t hI,size_t...I,typename hT,typename...T>
constexpr void bla(START<I...>){}

int main(int, char*[]) {
  START<'i','f','i',' ','f'>a{};

  print<nullptr>();
  printT<float>();
  printT<int>();
  printT<char>();
  return EXIT_SUCCESS;
}
