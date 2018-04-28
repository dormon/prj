#include <iostream>
#include <tuple>
#include <sstream>

template<typename T>
void printVariable(T const&t){
  std::cout << t.type.toStr() << " " << t.name << " = " << t.value.toStr() << std::endl;
}

template<typename T>
struct ValueWrapper{
  T data;
  ValueWrapper(T const&d):data(d){}
  std::string toStr()const{
    std::stringstream ss;
    ss << data;
    return ss.str();
  }
};

template <char... chars>
using tstring = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr tstring<chars...> operator""_tstr() {
  return {};
}

struct static_string{
  template<size_t N>
    constexpr static_string(const char(&str)[N])
    :m_data(&str[0]){}
  constexpr static_string(const char*str,size_t s):
    m_size(s),m_data(str){}
  constexpr static_string() = default;
  constexpr size_t size()const{return m_size;}
  constexpr const char*c_str()const{return m_data;}
  size_t m_size{0};
  const char*m_data = nullptr;
};

template<typename T,char...chars>
struct Parameter{
  constexpr Parameter(char const*n):name(n){}
  static_string name()
  const char *name = nullptr;
  using type = T;
};

template<typename T>
struct TypeWrapper{
  using type = T;
  std::string toStr()const{
    return typeid(T).name();
  }
};

template<typename T>
struct BasicVariable{
  BasicVariable(std::string const&n,T const&d):name(n),value(d){}
  std::string name;
  ValueWrapper<T>value;
  TypeWrapper<T>type;
};


int main() {
  Parameter<float>{"mas"_tsrt()};
  printVariable(BasicVariable{"mass",10.f});
  return 0;
}
