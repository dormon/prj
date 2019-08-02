#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>
#include <sstream>
#include <map>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

class Op{
  public:
    virtual void operator()(float*d)const = 0;
    enum Type{
      ADD,
      MUL,
      RELU,
      DIFFRELU,
      CONST,
    }type;
    Op(Type t):type(t){}
};

class Const: public Op{
  public:
    Const(size_t oo,float vv):Op(CONST),o(oo),v(vv){}
    size_t o;
    float v;
    void operator()(float*d){
      d[o] = v;
    }
};

class Add: public Op{
  public:
    Add(size_t oo,size_t aa,size_t bb):Op(ADD),o(oo),a(aa),b(bb){}
    size_t o;
    size_t a;
    size_t b;
    void operator()(float*d)const override{
      d[o] = d[a] + d[b];
    }
};

class Mul: public Op{
  public:
    Mul(size_t oo,size_t aa,size_t bb):Op(MUL),o(oo),a(aa),b(bb){}
    size_t o;
    size_t a;
    size_t b;
    void operator()(float*d)const override{
      d[o] = d[a] * d[b];
    }
};

class Relu: public Op{
  public:
    Relu(size_t oo,size_t aa):Op(RELU),o(oo),a(aa){}
    size_t o;
    size_t a;
    void operator()(float*d)const override{
      float v = d[a];
      if(v >= 0.f)d[o] = v;
      else d[o] = v*.1f;
    }
};

class DiffRelu: public Op{
  public:
    DiffRelu(size_t oo,size_t aa):Op(DIFFRELU),o(oo),a(aa){}
    size_t o;
    size_t a;
    void operator()(float*d)const override{
      float v = d[a];
      if(v >= 0.f)d[o] = v;
      else d[o] = v*10.f;
    }
};


class ProgramLayer{
  public:
    std::vector<Op>ops;
    void operator()(float*d)const{
      for(auto const&o:ops)
        o(d);
    }
};

class Program{
  public:
    std::vector<ProgramLayer>layers;
    void operator()(float*d)const{
      for(auto const&l:layers)
        l(d);
    }
};

class Layer{
  public:
    size_t inputSize;
    size_t outputSize;

    size_t weight;
    size_t bias  ;
    size_t weightX;
    size_t z;
    size_t o;
    size_t weightUpdate;
    size_t biasUpdate;
    size_t gz;
    size_t twbu;
};

class Net{
  public:
    Program prg;
    size_t input;
    std::vector<Layer>layers;
    std::vector<float>memory;
    void addLayer(size_t i){
      if(layers.empty())
        input = i;
      else{

      }
    }
};

int main(){

  return 0;
}


