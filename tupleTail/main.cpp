#include <iostream>
#include <tuple>

template<typename>
struct TupleTail{};

template<typename H,typename...T>
struct TupleTail<std::tuple<H,T...>>{
  using type = std::tuple<T...>;
};

void fce(typename TupleTail<std::tuple<int,float,std::string>>::type){}

int main() {
  using EmptyTuple = std::tuple<>;
  using TT = typename TupleTail<std::tuple<char,int,float>>::type;
  fce(std::tuple<float,std::string>(1.f,"asd"));
  return 0;
}
