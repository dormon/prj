#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

template <typename... Types>
struct foo {};

template <typename... Types1,
          template <typename...> class T,
          typename... Types2,
          template <typename...> class V,
          typename U>
void bar(const T<Types1...>&, const V<Types2...>&, const U& u) {
  std::cout << sizeof...(Types1) << std::endl;
  std::cout << sizeof...(Types2) << std::endl;
  std::cout << u << std::endl;
}

template <size_t... Is>
struct goo {};

template <size_t... T1,
          template <size_t...> typename T,
          size_t... T2,
          template <size_t...> typename V>
constexpr void par(T<T1...> const&, V<T2...> const&) {}

template<size_t...I>
constexpr goo<I...>dej(){}

int main() {
  foo<char, int, float> f1;
  foo<char, int>        f2;
  bar(f1, f2, 9);
  return 0;
}
