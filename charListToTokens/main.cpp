#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

template <char... chars>
using tstring = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr tstring<chars...> operator""_tstr() {
  return {};
}

template <typename>
struct X;

template <char... elements>
struct X<tstring<elements...>> {
  const char* GetString() const {
    static constexpr char str[sizeof...(elements) + 1] = {elements..., '\0'};
    return str;
  }
};

template<size_t H,size_t N,size_t...T>
constexpr void parse(){
  if constexpr(H>255)
    parse<
}


int main(int, char* []) {
  return EXIT_SUCCESS;
}
