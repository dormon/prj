#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

/*
class Object{};
class Property:Object{};

template<size_t N>
class Dimension:Property{};

template<typename T>
class Type:Object{};

class Scalar: Dimension<0>{};
class Vector: Dimension<1>{};
class Matrix: Dimension<2>{};

template<typename...Elements>
class Relation{};
class InputDomain:Object{};
class OutputDomain:Object{};
class Projection:InputDomain,OutputDomain{};
*/

using Vec3f = float[3];

template<typename Type,size_t Size>
class Vector{
  public:
    Type data[Size];
};

template<size_t...TAIL>
size_t product();

template<size_t HEAD,size_t...TAIL>
size_t prod(){
  return HEAD*product<TAIL...>();
};

template<size_t...TAIL>
size_t product(){
  return prod<TAIL...>();
};

template<>
size_t product<>(){
  return 1;
};

template<typename Type,size_t...Sizes>
class Tensor{
  public:
    Type data[product<Sizes...>()];
};
//class Vector

int main(){
  product<1,3>();
  //Tensor<float,10>a;
  return 0;
}


