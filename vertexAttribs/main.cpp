#include<cstdint>
#include<type_traits>
#include<cstring>
#include<array>

template<typename...T>
struct Attributes;

template<>struct Attributes<>{};

template<typename H>
struct Attributes<H>{
  H data;
  using Type = H;
};

template<typename H,typename...T>
struct Attributes<H,T...>{
  Attributes<H   >head;
  Attributes<T...>tail;
};

struct Vertex3Df{
  float data[3];
};

struct Vertex4Df{
  float data[4];
};

int main() {
  Attributes<Vertex3Df,Vertex3Df,std::array<Vertex4Df,2>>att;
  return 0;
}
