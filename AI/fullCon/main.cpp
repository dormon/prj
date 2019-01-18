#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>

float relu(float a){
  if(a<0)return 0.f;
  return a;
}

float diffRelu(float a){
  if(a>=0.0)return 1.f;
  return 0.f;
}

void mulMatVec(std::vector<float>&O,std::vector<float>const&M,std::vector<float>const&I){
  auto const is = I.size();
  auto const os = O.size();
  for(size_t o=0;o<os;++o){
    float r = 0;
    for(size_t i=0;i<is;++i)
      r += M[i+is*o]*I[i];
    O[o] = r;
  }
}

using OP1 = float(*)(float);
using OP2 = float(*)(float,float);

float add(float a,float b){
  return a+b;
}

float sub(float a,float b){
  return a-b;
}

float mul(float a,float b){
  return a*b;
}

void op(std::vector<float>&O,std::vector<float>const&A,std::vector<float>const&B,OP2 const&op){
  auto const s = A.size();
  for(size_t i=0;i<s;++i)
    O[i] = op(A[i],B[i]);
}

void op(std::vector<float>&O,std::vector<float>const&A,OP1 const&op){
  auto const s = A.size();
  for(size_t i=0;i<s;++i)
    O[i] = op(A[i]);
}

class Hidden{
  public:
    Hidden(size_t i,size_t o,OP1 const&a,OP1 const&d):activation(a),diff(d){
      weights.resize(i*o);
      output.resize(o);
      z.resize(o);
      biases.resize(o);
    }
    std::vector<float>const&get()const{
      return output;
    }
    void forward(std::vector<float>const&I){
      mulMatVec(z,weights,I);
      op(z,z,biases,add);
      op(output,z,activation);
    }
  protected:
    OP1 activation;
    OP1 diff      ;
    std::vector<float>output;
    std::vector<float>z;
    std::vector<float>weights;
    std::vector<float>biases;
};

class Sample{
  public:
    std::vector<float>input;
    std::vector<float>output;
};

class Network{
  public:
    Network(std::vector<size_t>const&c):conf(c){
      for(size_t l=1;l<c.size();++l)
        layers.push_back(Hidden(c[l-1],c[l],relu,diffRelu));
    }
    void forward(std::vector<float>const&I){
      layers.at(0).forward(I);
      for(size_t i=1;i<layers.size();++i)
        layers.at(i).forward(layers.at(0).get());
    }
    void trainStep(Sample const&sample){
      forward(sample.input);
      std::vector<float>ss(conf.back());
      op(ss,layers.back().get(),sample.output,sub);
    }
  protected:
    std::vector<size_t>conf;
    std::vector<Hidden>layers;
};

class Operation{
  public:
    std::vector<size_t>outputs;
    std::vector<size_t>inputs;
    enum Type{
      RELU,
      ADD,
      MUL,
    }type;
    Operation(std::vector<size_t>const&o,std::vector<size_t>const&i,Type t):outputs(o),inputs(i),type(t){}
};

class Program{
  public:
    std::vector<Operation>operations;
    std::vector<Program>toParallel()const{
      std::vector<Program>result;

      std::vector<std::vector<size_t>>varDepth;
      std::map<size_t,std::vector<size_t>>varToProducers;
      for(size_t i=0;i<operations.size();++i){
        auto const&op = operations.at(i);
        for(auto const&o:op.outputs){
          if(varToProducers.count(o) == 0)
            varToProducers[o] = {};
          varToProducers[o].push_back(i);
        }
      }

        
      return result;
    }
};

int main() { 

  return 0; 
}
