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

std::map<float*,size_t>sizes;

float*mynew(size_t n){
  auto r = new float[n];
  sizes[r] = n;
  return r;
}

void mydelete(float*d){
  sizes.erase(d);
  delete[]d;
}

size_t getSize(float*d){
  return sizes[d];
}

void measure(std::string const&n,std::function<void()>const&f){
  auto now  = std::chrono::high_resolution_clock::now();
  f();
  auto next  = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float>const elapsed = next - now;
  std::cerr << n << elapsed.count() << std::endl;
}

float uniformf(){
  return (float)rand() / (float)RAND_MAX;
}

float randomf(float mmin,float mmax){
  return uniformf()*(mmax-mmin)+mmin;
}


std::string vector2Str(float const*const d,size_t n){
  std::stringstream ss;
  ss << "[";
  for(size_t i=0;i<n;++i){
    if(i>0)ss << ",";
    ss << d[i];
  }
  ss << "]";
  return ss.str();
}

void randomize(float*const d,size_t n,float mmin,float mmax){
  assert(getSize(d) == n);
  for(size_t i=0;i<n;++i)
    d[i] = randomf(mmin,mmax);
}



float dot(float const*const a,float const*const b,size_t n){
  float res = 0;
  for(size_t i=0;i<n;++i)
    res += a[i] * b[i];
  return res;
}

void add(float*const o,float const*const a,float const*const b,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = a[i]+b[i];
}

void copy(float*const o,float const*const a,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = a[i];
}

void sub(float*const o,float const*const a,float const*const b,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = a[i]-b[i];
}

void mulMatrixVector(float*const c,float const*const m,float const*const v,size_t ni,size_t no){
  for(size_t o=0;o<no;++o)
    c[o] = dot(m+ni*o,v,ni);
}

void mulTrasposedMatrixVector(float*const c,float const*const m,float const*const v,size_t ni,size_t no){
  for(size_t o=0;o<no;++o){
    float acc = 0;
    for(size_t i=0;i<ni;++i)
      acc += m[i*no+o] * v[i];
    c[o] = acc;
  }
}

void componentMul(float*const o,float const*const a,float const*const b,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = a[i] * b[i];
}


void constMul(float*const o,float const*const a,float b,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = a[i] * b;
}

void matrixMul(float*const O,float const*const a,float const*const b,size_t an,size_t bn){
  for(size_t o=0;o<an;++o)
    constMul(O+o*bn,b,a[o],bn);
}

using Fce = std::function<float(float)>; 

constexpr const auto relu = [](float x){if(x>=0.f)return x;return x*0.1f;};
constexpr const auto diffRelu = [](float x){if(x>=0.f)return 1.f;return 0.1f;};

void apply(float*const o,float const*const a,Fce const&f,size_t n){
  for(size_t i=0;i<n;++i)
    o[i] = f(a[i]);
}

class Layer{
  public:
    Layer(
        size_t/**/      /**/input /**/ /**/        ,
        size_t/**/      /**/output/**/ /**/        ,
        Fce   /**/const&/**/fce   /**/=/**/relu    ,
        Fce   /**/const&/**/gce   /**/=/**/diffRelu):f(fce),g(gce),inputSize(input),outputSize(output)
    {
      alloc(input,output);
    }
    Layer(Layer const&oo){
      alloc(oo.input(),oo.output());
      copy(weight      ,oo.weight      ,oo.input()*oo.output());
      copy(bias        ,oo.bias        ,oo.output()           );
      copy(weightX     ,oo.weightX     ,oo.output()           );
      copy(z           ,oo.z           ,oo.output()           );
      copy(o           ,oo.o           ,oo.output()           );
      copy(weightUpdate,oo.weightUpdate,oo.input()*oo.output());
      copy(biasUpdate  ,oo.biasUpdate  ,oo.output()           );
      copy(gz          ,oo.gz          ,oo.output()           );
      copy(tw          ,oo.tw          ,oo.input()*oo.output());
      copy(twbu        ,oo.twbu        ,oo.output()           );
      inputSize = oo.input();
      outputSize = oo.output();
      f = oo.f;
      g = oo.g;
    }
    void alloc(size_t input,size_t output){
      weight       = new float[input*output];
      bias         = new float[output      ];
      weightX      = new float[output      ];
      z            = new float[output      ];
      o            = new float[output      ];
      weightUpdate = new float[input*output];
      biasUpdate   = new float[output      ];
      gz           = new float[output      ];
      tw           = new float[input*output];
      twbu         = new float[output      ];
    }
    ~Layer(){
      delete[]weight      ;
      delete[]bias        ;
      delete[]weightX     ;
      delete[]z           ;
      delete[]o           ;
      delete[]weightUpdate;
      delete[]biasUpdate  ;
      delete[]gz          ;
      delete[]tw          ;
      delete[]twbu        ;
    }
    void compute(float const*const x){
      mulMatrixVector(weightX,weight,x,inputSize,outputSize);
      add(z,weightX,bias,outputSize);
      apply(o,z,f,outputSize);
    }
    void forward(float const*const x){
      compute(x);
      apply(gz,z,g,outputSize);
    }
    void rand(float mmin,float mmax){
      randomize(bias,outputSize,mmin,mmax);
      randomize(weight,inputSize*outputSize,mmin,mmax);
    }
    size_t output()const{return outputSize;}
    size_t input()const{return inputSize;}
    float* weight      ;
    float* bias        ;
    Fce    f           ;
    Fce    g           ;
    float* weightX     ;
    float* z           ;
    float* o           ;
    float* weightUpdate;
    float* biasUpdate  ;
    float* gz          ;
    float* tw          ;
    float* twbu        ;
    size_t inputSize;
    size_t outputSize;
};

class Sample{
  public:
    Sample(size_t i,size_t o):inputSize(i),outputSize(o){
      alloc(i,o);
    }
    ~Sample(){
      delete[]x;
      delete[]y;
    }
    Sample(Sample const&o){
      alloc(o.inputSize,o.outputSize);
      copy(x,o.x,o.inputSize);
      copy(y,o.y,o.outputSize);
      inputSize = o.inputSize;
      outputSize = o.outputSize;
    }
    void alloc(size_t i,size_t o){
      x = new float[i];
      y = new float[o];
    }
    float*x;
    float*y;
    size_t inputSize;
    size_t outputSize;
};

class Network{
  public:
    size_t inputSize = 0;
    Network(size_t i):inputSize(i){}
    std::vector<Layer>layers;
    void forward(float const*const x){
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
    }
    ~Network(){
      if(C)delete[]C;
    }
    void compute(float*const O,float const*const i){
      bool first = true;
      Layer*prev = nullptr;
      for(auto&l:layers){
        if(first){
          first = false;
          l.compute(i);
        }else
          l.compute(prev->o);
        prev = &l;
      }
      copy(O,prev->o,prev->output());
    }
    float*C = nullptr;
    void backward(float const*const x,float const*const y,float s){
      sub(C,layers.back().o,y,layers.back().output());
      constMul(C,C,-2*s,layers.back().output());

      auto const computeUpdate = [&](size_t l,float const*const C){
        componentMul(layers[l].biasUpdate,C,layers[l].gz,layers[l].output());
        if(l == 0)
          matrixMul(layers[l].weightUpdate,layers[l].biasUpdate,x,layers[l].output(),layers[l].input());
        else
          matrixMul(layers[l].weightUpdate,layers[l].biasUpdate,layers[l-1].o,layers[l].output(),layers[l].input());
      };

      computeUpdate(layers.size()-1,C);

      for(size_t i=layers.size()-1;i>0;--i){
        auto&pl = layers.at(i);
        mulTrasposedMatrixVector(pl.twbu,pl.weight,pl.biasUpdate,pl.output(),pl.input());
        computeUpdate(i-1,pl.twbu);
      }
    }
    void update(){
      for(auto&l:layers){
        add(l.bias,l.bias,l.biasUpdate,l.output());
        add(l.weight,l.weight,l.weightUpdate,l.output()*l.input());
      }
    }
    void sampleTrain(Sample const&sample,float s){
      forward(sample.x);
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
      float*o  = new float[output()];
      float*oy = new float[output()];
      float counter = 0;
      for(size_t i=0;i<samples.size();++i){
        //std::cerr << "TEST: " << i << "/" << samples.size() << std::endl;
        compute(o,samples[i].x);
        sub(oy,o,samples[i].y,output());
        counter += dot(oy,o,output());
      }
      delete[]o;
      delete[]oy;
      return counter / samples.size();
    }
    void randomize(float mmin,float mmax){
      for(auto&l:layers)
        l.rand(mmin,mmax);
    }
    void pushLayer(size_t n,Fce const&f = relu,Fce const&g = diffRelu){
      if(layers.empty())
        layers.emplace_back(input(),n,f,g);
      else
        layers.emplace_back(layers.back().output(),n,f,g);
      if(C != nullptr)delete[]C;
      C = new float[n];
    }

    void operator()(float*const y,float const*const x){
      compute(y,x);
    }

    size_t input()const{return inputSize;}
    size_t output()const{return layers.back().output();}
};

int main(){
  srand(time(0));
  Network nn(2);
  nn.pushLayer(10);
  nn.pushLayer(10);
  nn.pushLayer(10);
  nn.pushLayer(10);
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
  auto trainSamples = genSamples(nn.input(),nn.output(),1,-2,2);
  auto testSamples  = genSamples(nn.input(),nn.output(),1,-2,2);

  nn.trainSGD(trainSamples,0.001f);
  //measure("traininig ",[&](){
  //  nn.trainSGD(trainSamples,0.001f);
  //});
  //std::cerr << "err: " << nn.test(testSamples) << std::endl;


  //
  //for(auto const&l:nn.layers){
  //  std::cerr << "bias: " << std::endl;
  //  std::cerr << l.bias << std::endl;
  //  std::cerr << "weights:" << std::endl;
  //  std::cerr << l.weight << std::endl;
  //}

  //std::vector<float>x(2);
  //std::vector<float>y(1);
  //x[0] = 1;
  //x[1] = 2;
  //nn(y.data(),x.data());
  //std::cerr << vector2Str(y.data(),y.size()) << std::endl;

  return 0;
}


