#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>


template <typename T>
T makej(T const& t);

template <>
float makej<float>(float const& t) {
  return t + t + t;
}

template <>
int32_t makej<int32_t>(int32_t const& t) {
  return t + t;
}














template <typename T>
T delej(T const& t) {
  if constexpr(std::is_same_v<T, float>)
    return t + t + t;
  else
    return t + t;
}


int main(int, char* []) {
  std::cout << "makej<float   >(): " << makej(3.f) << std::endl;
  std::cout << "makej<int32_t >(): " << makej(3  ) << std::endl;
  std::cout << "delej<float   >(): " << delej(3.f) << std::endl;
  std::cout << "delej<int32_t >(): " << delej(3  ) << std::endl;
  return EXIT_SUCCESS;
}
