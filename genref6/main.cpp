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

float uniformf(){
  return (float)rand() / (float)RAND_MAX;
}

float randomf(float mmin,float mmax){
  return uniformf()*(mmax-mmin)+mmin;
}

void randomize(float*v,float mmin,float mmax,size_t n){
  for(size_t i=0;i<n;++i)
    v[i] = randomf(mmin,mmax);
}

std::ostream& operator<< (std::ostream& stream, const std::vector<float>& v) {
  stream << "[";
  for(size_t i=0;i<v.size();++i){
    if(i>0)stream << ",";
    stream << v[i];
  }
  stream << "]";
  return stream;
}


void measure(std::string const&n,std::function<void()>const&f){
  auto now  = std::chrono::high_resolution_clock::now();
  f();
  auto next  = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float>const elapsed = next - now;
  std::cerr << n << elapsed.count() << std::endl;
}

using Fce = std::function<float(float)>; 

constexpr const auto relu = [](float x){if(x>=0.f)return x;return x*0.1f;};
constexpr const auto diffRelu = [](float x){if(x>=0.f)return 1.f;return 0.1f;};

void add(float*v,float const*a,float const*b,size_t n){
  for(size_t i=0;i<n;++i)
    v[i] = a[i] + b[i];
}

void sub(float*v,float const*a,float const*b,size_t n){
  for(size_t i=0;i<n;++i)
    v[i] = a[i] - b[i];
}

void vvvmul(float*v,float const*a,float const*b,size_t n){
  for(size_t i=0;i<n;++i)
    v[i] = a[i] * b[i];
}

void vvcmul(float*v,float const*a,float b,size_t n){
  for(size_t i=0;i<n;++i)
    v[i] = a[i] * b;
}

float dot(float const*a,float const*b,size_t n){
  float acc = 0;
  for(size_t i=0;i<n;++i)
    acc += a[i] * b[i];
  return acc;
}

void copy(float*c,float const*a,size_t n){
  for(size_t i=0;i<n;++i)
    c[i] = a[i];
}

void vmvmul(float*o,float const*m,float const*b,size_t r,size_t c){
  for(size_t i=0;i<r;++i)
    o[i] = dot(m+i*c,b,c);
}

void mulTransposed(float*c,float const*m,float const*b,size_t os,size_t is){
  for(size_t o=0;o<os;++o){
    float acc = 0;
    for(size_t i=0;i<is;++i)
      acc += m[i*os+o] * b[i];
    c[o] = acc;
  }
}

void mvvmul(float*o,float const*a,float const*b,size_t r,size_t c){
  for(size_t y=0;y<r;++y)
    for(size_t x=0;x<c;++x)
      o[y*c+x] = a[y] * b[x];
}

void apply(float*o,float const*v,Fce const&f,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = f(v[i]);
}

class LayerBase{
  public:
    virtual float*getOutput()const = 0;
};

class Input: public LayerBase{
  public:
    std::vector<float> &output;
    Input(std::vector<float> &i):output(i){}
    void setInput(std::vector<float> &i){output = i;}
    float* getOutput()const override{
      return output.data();
    }
};

class Layer: public LayerBase{
  public:
    Layer(
        size_t/**/      /**/input /**/ /**/        ,
        size_t/**/      /**/output/**/ /**/        ,
        Fce   /**/const&/**/fce   /**/=/**/relu    ,
        Fce   /**/const&/**/gce   /**/=/**/diffRelu):
      output      (output      ),
      input       (input       ),
      f           (fce         ),
      g           (gce         )
    {
      allocate(input,output);
    }
    Layer() = delete;
    Layer(Layer const&l){
      allocate(l.input,l.output);

      copy(weight      ,l.weight      ,l.input*l.output);
      copy(bias        ,l.bias        ,        l.output);
      copy(biasUpdate  ,l.biasUpdate  ,        l.output);
      copy(weightUpdate,l.weightUpdate,l.input*l.output);
      copy(weightX     ,l.weightX     ,        l.output);
      copy(z           ,l.z           ,        l.output);
      copy(o           ,l.o           ,        l.output);
      copy(gz          ,l.gz          ,        l.output);
      output = l.output;
      input = l.input;
      f = l.f;
      g = l.g;
    }
    void allocate(size_t input,size_t output){
      weight       = new float[input*output];
      bias         = new float[      output];
      biasUpdate   = new float[      output];
      weightUpdate = new float[input*output];
      weightX      = new float[      output];
      z            = new float[      output];
      o            = new float[      output];
      gz           = new float[      output];
      twbu         = new float[input       ];
    }
    ~Layer(){
      delete[]weight      ;
      delete[]bias        ;
      delete[]biasUpdate  ;
      delete[]weightUpdate;
      delete[]weightX     ;
      delete[]z           ;
      delete[]o           ;
      delete[]gz          ;
      delete[]twbu        ;
    }
    float* getOutput()const override{
      return o;
    }
    float* compute(float const*x){
      vmvmul(weightX,weight,x,output,input);
      add(z,weightX,bias,output);
      apply(o,z,f,output);
      return o;
    }
    float* forward(float const*x){
      compute(x);
      apply(gz,z,g,output);
      return o;
    }
    void randomize(float mmin,float mmax){
      ::randomize(bias,mmin,mmax,output);
      ::randomize(weight,mmin,mmax,input*output);
    }
    size_t output;
    size_t input ;
    float*weight=nullptr;
    float*bias  =nullptr;
    Fce    f           ;
    Fce    g           ;
    float*weightX = nullptr;
    float*z = nullptr          ;
    float*o = nullptr          ;
    float*weightUpdate = nullptr;
    float*biasUpdate  = nullptr;
    float*gz          = nullptr;
    float*twbu        = nullptr;
};

class Sample{
  public:
    Sample(size_t i,size_t o):x(i),y(o){}
    std::vector<float> x;
    std::vector<float> y;
};

class Network{
  public:
    size_t inputSize = 0;
    Network(size_t i):inputSize(i),C(i){}
    std::vector<Layer>layers;
    float* forward(float const*x){
      bool first = true;
      Layer*prev;
      for(auto&l:layers){
        if(first){
          first = false;
          l.forward(x);
        }else
          l.forward(prev->o);
        prev = &l;
      }
      return prev->o;
    }
    void compute(std::vector<float>&o,std::vector<float> const&i){
      bool first = true;
      Layer*prev = nullptr;
      for(auto&l:layers){
        if(first){
          first = false;
          l.compute(i.data());
        }else
          l.compute(prev->o);
        prev = &l;
      }
      copy(o.data(),prev->o,o.size());
    }
    std::vector<float> C;
    void backward(std::vector<float> const&x,std::vector<float> const&y,float s){
      sub(C.data(),layers.back().o,y.data(),y.size());
      vvcmul(C.data(),C.data(),-2*s,C.size());

      auto const computeUpdate = [&](size_t l,float const*C){
        auto&ll           = layers[l];
        auto&biasUpdate   = ll.biasUpdate  ;
        auto&gz           = ll.gz          ;
        auto&weightUpdate = ll.weightUpdate;
        vvvmul(biasUpdate,C,gz,ll.output);
        if(l == 0)
          mvvmul(weightUpdate,biasUpdate,x.data(),ll.output,x.size());
        else
          mvvmul(weightUpdate,biasUpdate,layers[l-1].o,ll.output,layers[l-1].output);
      };

      computeUpdate(layers.size()-1,C.data());

      for(size_t i=layers.size()-1;i>0;--i){
        auto&pl = layers.at(i);
        mulTransposed(pl.twbu,pl.weight,pl.biasUpdate,pl.input,pl.output);
        computeUpdate(i-1,pl.twbu);
      }
    }
    void update(){
      for(auto&l:layers){
        add(l.bias,l.bias,l.biasUpdate,l.output);
        add(l.weight,l.weight,l.weightUpdate,l.input*l.output);
      }
    }
    void sampleTrain(Sample const&sample,float s){
      forward(sample.x.data());
      backward(sample.x,sample.y,s);
      update();
    }
    void trainSGD(std::vector<Sample>const&samples,float speed){
      for(size_t i=0;i<samples.size();++i){
        //std::cerr << "SGD: " << i << "/" << samples.size() << std::endl;
        sampleTrain(samples[i],speed);
      }
    }
    float test(std::vector<Sample>const&samples){
      std::vector<float> o(output());
      std::vector<float> oy(output());
      float counter = 0;
      for(size_t i=0;i<samples.size();++i){
        //std::cerr << "TEST: " << i << "/" << samples.size() << std::endl;
        compute(o,samples[i].x);
        sub(oy.data(),o.data(),samples[i].y.data(),o.size());
        counter += dot(oy.data(),oy.data(),oy.size());
      }
      return counter / samples.size();
    }
    void randomize(float mmin,float mmax){
      for(auto&l:layers)
        l.randomize(mmin,mmax);
    }
    void pushLayer(size_t n,Fce const&f = relu,Fce const&g = diffRelu){
      if(layers.empty())
        layers.emplace_back(input(),n,f,g);
      else
        layers.emplace_back(layers.back().output,n,f,g);
      C = std::vector<float>(n);
    }

    std::vector<float> operator()(std::vector<float> const&x){
      std::vector<float> y(output());
      compute(y,x);
      return y;
    }

    size_t input()const{return inputSize;}
    size_t output()const{return layers.back().output;}
};

int main(){
  srand(time(0));
  Network nn(2);
  nn.pushLayer(20);
  nn.pushLayer(20);
  nn.pushLayer(20);
  nn.pushLayer(20);
  nn.pushLayer(20);
  nn.pushLayer(1);

  nn.randomize(-1,1);
  auto const genSamples = [](size_t input,size_t output,size_t N,float mmin,float mmax){
    std::vector<Sample>samples;
    for(size_t i=0;i<N;++i){
      samples.push_back(Sample(input,output));
      auto&s = samples.back();
      s.x[0] = randomf(mmin,mmax);
      s.x[1] = randomf(mmin,mmax);
      s.y[0] = s.x[0] + s.x[1];
    }
    return samples;
  };
  auto trainSamples = genSamples(nn.input(),nn.output(),100000,-2,2);
  auto testSamples  = genSamples(nn.input(),nn.output(),1000,-2,2);

  measure("traininig ",[&](){
    nn.trainSGD(trainSamples,0.001f);
  });
  std::cerr << "err: " << nn.test(testSamples) << std::endl;
  //
  //for(auto const&l:nn.layers){
  //  std::cerr << "bias: " << std::endl;
  //  std::cerr << l.bias << std::endl;
  //  std::cerr << "weights:" << std::endl;
  //  std::cerr << l.weight << std::endl;
  //}

  std::vector<float> x(2);
  x[0] = 1;
  x[1] = 2;
  std::cerr << nn(x) << std::endl;

  return 0;
}


