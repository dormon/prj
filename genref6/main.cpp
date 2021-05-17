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

enum class LayerType{INPUT,DEEP,OUTPUT};

class Layer{
  public:
    void create(size_t i,size_t o,LayerType t = LayerType::DEEP,Fce const&fce=relu,Fce const&gce=diffRelu){
      output = o;
      input  = i;
      f = fce;
      g = gce;
      layerType = t;
      allocate();
    }
    void allocate(){
      if(layerType != LayerType::INPUT){
        weight       = new float[input*output];
        bias         = new float[      output];
        biasUpdate   = new float[      output];
        weightUpdate = new float[input*output];
        weightX      = new float[      output];
        z            = new float[      output];
        if(layerType != LayerType::OUTPUT)
          o            = new float[      output];
        gz           = new float[      output];
        twbu         = new float[input       ];
      }
    }
    void free(){
      if(layerType != LayerType::INPUT){
        delete[]weight      ;
        delete[]bias        ;
        delete[]biasUpdate  ;
        delete[]weightUpdate;
        delete[]weightX     ;
        delete[]z           ;
        if(layerType != LayerType::OUTPUT)
          delete[]o           ;
        delete[]gz          ;
        delete[]twbu        ;
      }
    }
    float const* compute(float const*x){
      if(layerType==LayerType::INPUT)return o;
      vmvmul(weightX,weight,x,output,input);
      add(z,weightX,bias,output);
      apply(o,z,f,output);
      return o;
    }
    float const* forward(float const*x){
      compute(x);
      apply(gz,z,g,output);
      return o;
    }
    void randomize(float mmin,float mmax){
      if(layerType==LayerType::INPUT)return;
      ::randomize(bias,mmin,mmax,output);
      ::randomize(weight,mmin,mmax,input*output);
    }
    void update(){
      if(layerType==LayerType::INPUT)return;
      add(bias,bias,biasUpdate,output);
      add(weight,weight,weightUpdate,input*output);
    }
    size_t output;
    size_t input ;
    LayerType layerType;
    Fce       f;
    Fce       g;
    float*weight       = nullptr;
    float*bias         = nullptr;
    float*weightX      = nullptr;
    float*z            = nullptr;
    float*o            = nullptr;
    float*weightUpdate = nullptr;
    float*biasUpdate   = nullptr;
    float*gz           = nullptr;
    float*twbu         = nullptr;
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
    Network(std::vector<size_t>const&ls,Fce const&f = relu,Fce const&g = diffRelu):inputSize(ls.front()),C(ls.back()){
      layers.resize(ls.size());
      for(size_t i=0;i<ls.size();++i)
        layers[i].create(i>0?ls[i-1]:0,ls[i],i==0?LayerType::INPUT:LayerType::DEEP);
    }
    ~Network(){
      for(auto&l:layers)
        l.free();
      layers.clear();
    }
    std::vector<Layer>layers;
    float const* forward(){
      for(size_t i=1;i<layers.size();++i)
        layers[i].forward(layers[i-1].o);
      return layers.back().o;
    }
    void compute(std::vector<float>&o,std::vector<float> const&i){
      layers.front().o = (float*)i.data();
      for(size_t i=1;i<layers.size();++i)
        layers[i].compute(layers[i-1].o);
      copy(o.data(),layers.back().o,o.size());
    }
    std::vector<float> C;
    void backward(std::vector<float> const&y,float s){
      sub(C.data(),layers.back().o,y.data(),y.size());
      vvcmul(C.data(),C.data(),-2*s,C.size());

      auto const computeUpdate = [&](size_t l,float const*C){
        auto&ll           = layers[l];
        auto&biasUpdate   = ll.biasUpdate  ;
        auto&gz           = ll.gz          ;
        auto&weightUpdate = ll.weightUpdate;
        vvvmul(biasUpdate,C,gz,ll.output);
        mvvmul(weightUpdate,biasUpdate,layers[l-1].o,ll.output,layers[l-1].output);
      };

      computeUpdate(layers.size()-1,C.data());

      for(size_t i=layers.size()-1;i>1;--i){
        auto&pl = layers.at(i);
        mulTransposed(pl.twbu,pl.weight,pl.biasUpdate,pl.input,pl.output);
        computeUpdate(i-1,pl.twbu);
      }
    }
    void update(){
      for(auto&l:layers)l.update();
    }
    void sampleTrain(Sample const&sample,float s){
      layers.front().o = (float*)sample.x.data();
      forward();
      backward(sample.y,s);
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

    std::vector<float> operator()(std::vector<float> const&x){
      std::vector<float> y(output());
      compute(y,x);
      return y;
    }

    size_t input()const{return inputSize;}
    size_t output()const{return layers.back().output;}
};

int main(){
  //srand(time(0));
  srand(0);
  auto nn = Network({2,20,20,20,20,1});

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
  auto oo = nn(x);
  std::cerr << oo << std::endl;

  return 0;
}


