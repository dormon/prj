#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <array>
#include <vector>
#include <functional>



template<size_t N,typename T = float>
using Vector = std::array<T,N>;

template<size_t X,size_t Y,typename T = float>
using Matrix = std::array<Vector<X,T>,Y>;

template<size_t N,typename T = float>
T dot(Vector<N,T>const&a,Vector<N,T>const&b){
  T result = 0;
  for(size_t i=0;i<N;++i)
    result += a.at(i) * b.at(i);
  return result;
}

template<size_t N,typename T = float>
Vector<N,T> operator+(Vector<N,T>const&a,Vector<N,T>const&b){
  Vector<N,T> result;
  for(size_t i=0;i<N;++i)
    result.at(i) = a.at(i) + b.at(i);
  return result;
}

template<size_t N,typename T = float>
Vector<N,T> operator-(Vector<N,T>const&a,Vector<N,T>const&b){
  Vector<N,T> result;
  for(size_t i=0;i<N;++i)
    result.at(i) = a.at(i) - b.at(i);
  return result;
}


template<size_t N,typename T = float>
Vector<N,T> operator*(Vector<N,T>const&a,T const&b){
  Vector<N,T> result;
  for(size_t i=0;i<N;++i)
    result.at(i) = a.at(i) * b;
  return result;
}

template<size_t X,size_t Y,typename T = float>
Vector<Y,T> operator*(Matrix<X,Y,T>const&m,Vector<X,T>const&v){
  Vector<Y,T>result;
  for(size_t y=0;y<Y;++y)
    result.at(y) = dot(m.at(y),v);
  return result;
}

template<size_t X,size_t Y,typename T = float>
Matrix<X,Y,T> operator*(Matrix<X,Y,T>const&m,T const&v){
  Matrix<X,Y,T>result;
  for(size_t y=0;y<Y;++y)
    result.at(y) = m.at(y)*v;
  return result;
}


template<size_t X,size_t Y,typename T = float>
Matrix<X,Y,T> operator+(Matrix<X,Y,T>const&a,Matrix<X,Y,T>const&b){
  Matrix<X,Y,T>result;
  for(size_t y=0;y<Y;++y)
    result.at(y) = a.at(y) + b.at(y);
  return result;
}

template<size_t N,typename T = float>
Vector<N,T> apply(Vector<N,T>const&v,std::function<float(float)>const&f){
  Vector<N,T>result;
  for(size_t i=0;i<N;++i)
    result.at(i) = f(v.at(i));
  return result;
}

template<size_t I,size_t O,typename T = float>
struct DenseParameters{
  Matrix<I,O,T>weights;
  Vector<O,T>biases;
};

template<size_t I,size_t O,typename T = float>
DenseParameters<I,O,T>operator*(DenseParameters<I,O,T>const&p,float a){
  DenseParameters<I,O,T>result;
  result.weights = p.weights * a;
  result.biases  = p.biases * a;
  return result;
}

template<size_t I,size_t O,typename T = float>
DenseParameters<I,O,T>operator+=(DenseParameters<I,O,T>&a,DenseParameters<I,O,T>const&b){
  a.weights = a.weights + b.weights;
  b.biases = a.biases + b.biases;
  return a;
}

template<size_t I,size_t O,typename T = float>
struct TrainSample{
  Vector<I,T>x;
  Vector<O,T>y;
};

template<size_t I,size_t O,typename T = float>
Vector<O,T>oneLayer(Vector<I,T>const&x,DenseParameters<I,O,T> const&p){
  apply(p.weights*x + p.biases,[](float x){return x>0.f?x:0.f;});
}

template<size_t I,size_t O,typename T = float>
DenseParameters<I,O,T>getDerivation(DenseParameters<I,O,T>old,TrainSample<I,O,T>const&sample){
  // c(A,b) = (f(Ax+b)-y)^2
  // c(A,b)'b = 2*(f(Ax+b)-y) * f(Ax+b) * 1
  // c(A,b)'A = 2*(f(Ax+b)-y) * f(Ax+b) * x
  //
  // c(A,b) =
  //
  // ( f( [a00 a10 a20] * [x0] + [b0] ) - [y0] ) ^ 2
  // (  ( [a01 a11 a21]   [x1]   [b1] )   [y1] )
  // (  (                 [x2]        )        ) ^ 2
  //
  // =
  //
  // ( f( [a00 a10 a20] * [x0] + b0 ) - y0 ) ^ 2
  // (  (                 [x1]      )      )
  // (  (                 [x2]      )      )
  // +
  // ( f( [a01 a11 a21] * [x0] + b1 ) - y1 ) ^ 2
  // (  (                 [x1]      )      )
  // (  (                 [x2]      )      )
  //
  // =
  //
  // (f(a_0 * x + b0) - y0) ^ 2
  // +
  // (f(a_1 * x + b1) - y1) ^ 2
  //
  // c(A,b)'b0 = 2*(f(a_0 * x + b0) - y0) * g(a_0 * x + b0) * 1
  // c(A,b)'b1 = 2*(f(a_1 * x + b1) - y1) * g(a_1 * x + b1) * 1
  //
  // c(A,b)'b = 2*(f(Ax+b)-y) . g(Ax+b)
  //
  // c(A,b)'a00 = 2*(f(a_0 * x + b0) - y0) * g(a_0 * x + b0) * x0
  // c(A,b)'a10 = 2*(f(a_0 * x + b0) - y0) * g(a_0 * x + b0) * x1
  // c(A,b)'a20 = 2*(f(a_0 * x + b0) - y0) * g(a_0 * x + b0) * x2
  // c(A,b)'a_0 = 2*(f(a_0 * x + b0) - y0) * g(a_0 * x + b0) * x
  // c(A,b)'a_1 = 2*(f(a_1 * x + b1) - y1) * g(a_1 * x + b1) * x
  // c(A,b)'a   = 2*(f(Ax+b)-y) TODO
  //
  DenseParameters<I,O,T>result;
  auto computeOutput = oneLayer(sample.x,old);
  auto outputDifference = computeOutput - sample.y;

  return result;
}

template<size_t I,size_t O,typename T = float>
DenseParameters<I,O,T>trainOneLayer(std::vector<TrainSample<I,O,T>>const&dataset,float speed = 1.f,size_t steps = 100){
  DenseParameters<I,O,T>result;
  DenseParameters<I,O,T>oldResult = result;
  for(size_t step=0;step<steps;++step){
    DenseParameters<I,O,T> derivation;
    for(auto const&sample:dataset)
      derivation += getDerivation(oldResult,sample);
    result = oldResult -speed*derivation;
    oldResult = result;
  }
}

template<size_t N,typename T = float>
std::ostream& operator<<(std::ostream&o,Vector<N,T> const&a){
  o << "[";
  bool first = true;
  for(auto const&x:a){
    if(first)first = false;
    else o << ",";
    o << x;
  }
  o << "]";
  return o;
}

template<size_t X,size_t Y,typename T = float>
std::ostream& operator<<(std::ostream&o,Matrix<X,Y,T>const&m){
  o << "[";
  bool first = true;
  for(auto const&x:m){
    if(first)first = false;
    else o << ",";
    o << x;
  }
  o << "]";
  return o;
}



int main(int, char*[]) {
  Vector<3>a = {1,2,3};
  Vector<3>b = {1,2,3};
  Matrix<3,3>c = {{{1,0,0},{0,1,0},{0,0,1}}};
  std::cout << a << std::endl;
  std::cout << c << std::endl;
  std::cout << dot(a,b) << std::endl;
  return EXIT_SUCCESS;
}
