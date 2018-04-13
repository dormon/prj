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

using Base = size_t;

using Function = void (*)(uint8_t*);

struct If {
  Function fce;
  float*   condition;
};

void f_if(uint8_t* param) {
  auto const iff = reinterpret_cast<If*>(param);
  if (*iff->condition != 0.f) (iff->fce)(param + sizeof(iff));
}

struct While {
  Function fce;
  float*   condition;
};

void f_while(uint8_t* param) {
  auto const whilef = reinterpret_cast<While*>(param);
  while (*whilef->condition != 0.f) whilef->fce(param + sizeof(Function));
}

struct Pair {
  Function fceA;
  Function fceB;
  size_t   offset;
};

void f_pair(uint8_t* param) {
  auto const pairf = reinterpret_cast<Pair*>(param);
  pairf->fceA(param + sizeof(Pair));
  if (pairf->fceB) pairf->fceB(param + sizeof(Pair) + pairf->offset);
}

#define TRANSITION(CHAR, STATE)          \
  if constexpr (FIRST_CHAR == CHAR) do { \
      parser<STATE, REST_CHARS...>();    \
      return;                            \
  } while (false)

enum State {
  START,
  I_STATE,
  IN_STATE,
  INT_STATE,
  F_STATE,
  FL_STATE,
  FLO_STATE,
  FLOA_STATE,
  FLOAT_STATE,
};

template <State STATE>
constexpr void parser() {
  static_assert(STATE != START, "failed parsing");
}

template <State STATE, char FIRST_CHAR, char... REST_CHARS>
constexpr void parser() {
  if constexpr (STATE == START) {
    TRANSITION(' ', START);
    TRANSITION('i', I_STATE);
    TRANSITION('f', F_STATE);
    return;
  }
  if constexpr (STATE == I_STATE) {
    TRANSITION('n', IN_STATE);
    return;
  }
  if constexpr (STATE == IN_STATE) {
    TRANSITION('t', INT_STATE);
    return;
  }
  if constexpr (STATE == INT_STATE) {
    TRANSITION(' ', START);
    return;
  }
  if constexpr (STATE == F_STATE) {
    TRANSITION('l', FL_STATE);
    return;
  }
  if constexpr (STATE == FL_STATE) {
    TRANSITION('o', FLO_STATE);
    return;
  }
  if constexpr (STATE == FLO_STATE) {
    TRANSITION('a', FLOA_STATE);
    return;
  }
  if constexpr (STATE == FLOA_STATE) {
    TRANSITION('t', FLOAT_STATE);
    return;
  }
}

template <char a, char b, char c>
using isInt =
    typename std::enable_if<a == 'i' && b == 'n' && c == 't', int>::type;

namespace detail {
template <typename T, typename Enable, typename... Args>
class A_impl;

template <typename T, typename... Args>
class A_impl<T, typename std::enable_if<T::value>::type, Args...> {
  // code here
};

template <typename T, typename... Args>
class A_impl<T, typename std::enable_if<!T::value>::type, Args...> {
  // code here
};
}  // namespace detail

template <typename T, typename... Args>
class A : public detail::A_impl<T, void, Args...> {};



int main(int, char* []) {
  X<decltype("my_string"_tstr)>{};

  parser<START, ' ', 'i', 'n', 't', ' ', 'f', 'l', 'o', 'a', 't'>();
  return EXIT_SUCCESS;
}
