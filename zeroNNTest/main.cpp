#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

float f(float x){
  if(x<0)return 0;
  return x;
}

float g(float x){
  if(x<0)return 0;
  return 1;
}

float uniform(){
  return (float)rand() / (float)RAND_MAX;
}

void train(){
  float A=uniform();
  float a=uniform();
  float B=uniform();
  float b=uniform();
  float x=1;
  float y=1;

  // (f(Bf(Ax+a)+b)-y)^2
  // b: -2(f(Bf(Ax+a)+b)-y)*g(Bf(Ax+a)+b)
  // B: -2(f(Bf(Ax+a)+b)-y)*g(Bf(Ax+a)+b)*f(Ax+a)
  // a: -2(f(Bf(Ax+a)+b)-y)*g(Bf(Ax+a)+b)*B*g(Ax+a)
  // A: -2(f(Bf(Ax+a)+b)-y)*g(Bf(Ax+a)+b)*B*g(Ax+a)*x
  //
  auto const nn = [&](float x){return f(B*f(A*x+a)+b);};
  for(size_t i=0;i<10;++i){
    std::cerr << A << " " << a << " " << B << " " << b << std::endl;
    std::cerr << y << " = nn(" << x << ") <=> " << nn(x) << std::endl;
    float speed = 0.01f;
    float bu = -2*speed*(f(B*f(A*x+a)+b)-y)*g(B*f(A*x+a)+b);
    float Bu = -2*speed*(f(B*f(A*x+a)+b)-y)*g(B*f(A*x+a)+b)*f(A*x+a);
    float au = -2*speed*(f(B*f(A*x+a)+b)-y)*g(B*f(A*x+a)+b)*B*f(A*x+a);
    float Au = -2*speed*(f(B*f(A*x+a)+b)-y)*g(B*f(A*x+a)+b)*B*f(A*x+a)*x;
    b += bu;
    B += Bu;
    a += au;
    A += Au;
  }
}

int main(){
  train();
  return 0;
}


