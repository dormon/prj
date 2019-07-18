#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <array>

float uniformf(){
  return (float)rand() / (float)RAND_MAX;
}

float randomf(float mmin,float mmax){
  return uniformf()*(mmax-mmin)+mmin;
}


using Fce = std::function<float(float)>; 

constexpr const auto relu = [](float x){if(x>=0.f)return x;return x*0.1f;};
constexpr const auto diffRelu = [](float x){if(x>=0.f)return 1.f;return 0.1f;};
constexpr const auto invrelu = [](float x){if(x>=0.f)return x;return x*10.f;};

// G0 = y-f(Ax+a)
// G1 = F(G0)
// Ga = G1-Ax
// G2 = G1-a
// GA = G2/x
//
// (y-f(Ax+a))^2
// 2(y-f(Ax+a))*g(Ax+a)*x A
// 2(y-f(Ax+a))*g(Ax+a)   a

int main(){
  srand(time(0));
  using Par = std::array<float,4>;

  auto const abs = [](float x){if(x<0)return-x;return x;};
  auto const dist = [&](Par const&p){return abs(p[1]-relu(p[2]*p[0]+p[3]));};

  float speed = 0.001f;

  std::vector<std::function<void(Par&p)>>methods;
  methods.push_back(
  [&](Par&p){
    auto&x = p[0];
    auto&y = p[1];
    auto&A = p[2];
    auto&a = p[3];
    float G0 = y - relu(A*x+a);
    float G1 = invrelu(G0);
    float Ga = G1-A*x;
    float G2 = G1-a;
    float GA = G2/x;
    A += GA * speed;
    a += Ga * speed;
  });
  methods.push_back(
  [&](Par&p){
    auto&x = p[0];
    auto&y = p[1];
    auto&A = p[2];
    auto&a = p[3];
    float au = 2*(y-relu(A*x+a))*diffRelu(A*x+a);
    float Au = 2*(y-relu(A*x+a))*diffRelu(A*x+a)*x;
    A += Au * speed;
    a += au * speed;
  });
  methods.push_back(
  [&](Par&p){
    auto&x = p[0];
    auto&y = p[1];
    auto&A = p[2];
    auto&a = p[3];


    float G0 = y - relu(A*x+a);
    float G1 = invrelu(G0);
    float G2 = G1 - (A*x+a);
    float Au = G2 / (1-x*x);
    float au = G2 - Au*x;
    a += au * speed;
    A += Au * speed;
    //G1 : A*x+a
    //G1 = (A+Au)*x + (a+au)
    //G1 = A*x+a + Au*x+au
    //G1 - A*x-a = Au*x+au
    //G2 = Au*x+au
    //ua = G2-Au*x
    //Au^2 + (G2-Au*x)^2
    //2Au + 2(G2-Au*x)*x = 0
    //2Au + 2G2-2Aux^2 = 0
    //Au(2-2x^2)+2G2 = 0
    //Au = 2G2 / (2-2x^2)
    //Au = G2 / (1-x^2)
    //
    //
  });
  methods.push_back(
  [&](Par&p){
    auto&x = p[0];
    auto&y = p[1];
    auto&A = p[2];
    auto&a = p[3];

    float G0 = y - relu(A*x+a);
    float G1 = invrelu(G0);
    float G2 = G1 - (A*x+a);
    //G2 = Au*x+au
    float Au = randomf(-1,1);
    float au = G2 - Au*x;
    a += au * speed;
    A += Au * speed;
  });
  methods.push_back(
  [&](Par&p){
    auto&x = p[0];
    auto&y = p[1];
    auto&A = p[2];
    auto&a = p[3];

    // inv(y) = (A+Au)x+(a+au)
    // inv(y)-Ax-a = Aux+au
    // G0 = Aux+au
    // au = G0-Aux
    float G0 = invrelu(y) - A*x-a;
    float Au = randomf(-1,1);
    float au = G0 - Au*x;
    a += au * speed;
    A += Au * speed;
  });

  std::vector<Par>scenarios;
  scenarios.push_back({randomf(-10,10),randomf(-10,10),randomf(-10,10),randomf(-10,10)});
  while(scenarios.size() != methods.size())
    scenarios.push_back(scenarios.back());

  std::vector<std::string>names = {"gradProp","diff","var0","var1,var2"};

  std::cerr << "id";
  for(auto const&n:names){
    std::cerr << "," << n;
  }
  std::cerr << std::endl;
  

  auto const newTrain = [&](){
    for(size_t i=0;i<1000;++i){
      std::cerr << i;
      for(auto const&s:scenarios)
        std::cerr << "," << dist(s);
      std::cerr << std::endl;

      for(size_t i=0;i<methods.size();++i)
        methods[i](scenarios[i]);
    }
  };


  newTrain();

  return 0;
}


