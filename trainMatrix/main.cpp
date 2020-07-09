#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>

float randf(){
  return (float)rand() / (float)RAND_MAX;
}

template<typename T = float>
using Vector = std::vector<T>;

using VecF = Vector<float>;

template<typename T = float>
void add(Vector<T> &out,Vector<T> const&a,Vector<T> const&b){
  for(size_t i=0;i<a.size();++i)
    out[i] = a[i] + b[i];
}

template<typename T = float>
float dot(Vector<T>const&a,Vector<T>const&b,size_t n,size_t ao=0,size_t bo=0){
  float r = 0.f; 
  for(size_t i=0;i<n;++i)
    r += a[ao+i]*b[bo+i];
  return r;
}

template<typename T = float>
void mul(Vector<T>&out,Vector<T>const&M,Vector<T>const&V){
  for(size_t o=0;o<out.size();++o)
    out[o] = dot(M,V,V.size(),o*V.size());
}


void test(){
  float M[4];
  float X[2];
  float Y[2];

  for(int i=0;i<4;++i)M[i] = randf();
  for(int i=0;i<2;++i)X[i] = randf();
  for(int i=0;i<2;++i)Y[i] = randf();


}

int main()
{

  return 0;
}
