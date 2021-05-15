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

class Vector{
  public:
    Vector(float*d,size_t s):data(d),size(s),allocated(false){}
    Vector(float const*d,size_t s):data(const_cast<float*>(d)),size(s),allocated(false){}
    Vector(size_t s):size(s),allocated(true){data = new float[s];}
    Vector(Vector const&o){copy(o);}
    Vector&operator=(Vector const&o){
      free();
      copy(o);
      return*this;
    }
    float&operator[](size_t i){
      return data[i];
    }
    float const&operator[](size_t i)const{
      return data[i];
    }
    ~Vector(){
      free();
    }
    void randomize(float mmin,float mmax){
      for(size_t i=0;i<size;++i)
        data[i] = randomf(mmin,mmax);
    }
    void free(){
      if(allocated)delete[]data;
      data = nullptr;
      size = 0;
      allocated = false;
    }
    void copy(Vector const&o){
      if(o.allocated){
        data = new float[o.size];
        memcpy(data,o.data,sizeof(float)*o.size);
      }else
        data = o.data;
      size = o.size;
      allocated = o.allocated;
    }
    float* data      = nullptr;
    size_t size      = 0      ;
    bool   allocated = false  ;
};

class Matrix: public Vector{
  public:
    Matrix(size_t r,size_t c):Vector(r*c),rows(r),columns(c){}
    Matrix(float*d,size_t r,size_t c):Vector(d,r*c),rows(r),columns(c){}
    Matrix(Matrix const&o):Vector(o),rows(o.rows),columns(o.columns){}
    Matrix&operator=(Matrix const&o){
      free();
      copy(o);
      rows = o.rows;
      columns = o.columns;
      return*this;
    }
    //Vector operator[](size_t r){
    //  return Vector(data + columns*r,columns);
    //}
    Vector const operator[](size_t r)const{
      return Vector(data + columns*r,columns);
    }
    size_t rows    = 0;
    size_t columns = 0;
};

std::ostream& operator<< (std::ostream& stream, const Vector& v) {
  stream << "[";
  for(size_t i=0;i<v.size;++i){
    if(i>0)stream << ",";
    stream << v.data[i];
  }
  stream << "]";
  return stream;
}

std::ostream& operator<< (std::ostream& stream, const Matrix& v) {
  for(size_t r=0;r<v.rows;++r)
    stream << v[r] << std::endl;
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

void copy(Vector&c,Vector const&a){
  for(size_t i=0;i<c.size;++i)
    c[i] = a[i];
}

void add(Matrix&c,Matrix const&a,Matrix const&b){
  for(size_t i=0;i<a.size;++i)
    c.data[i] = a.data[i] + b.data[i];
}

void vmvmul(Vector&c,Matrix const&a,Vector const&b){
  for(size_t i=0;i<c.size;++i)
    //c.data[i] = dot(a[i].data,b.data,b.size);
    c.data[i] = dot(a.data+i*a.columns,b.data,b.size);
}

void mulTransposed(Vector&c,Matrix const&a,Vector const&b){
  for(size_t o=0;o<c.size;++o){
    float acc = 0;
    for(size_t i=0;i<b.size;++i)
      acc += a.data[i*c.size+o] * b.data[i];
    c.data[o] = acc;
  }
}

void mvvmul(Matrix&o,Vector const&a,Vector const&b){
  for(size_t r=0;r<o.rows;++r)
    for(size_t c=0;c<o.columns;++c)
      o.data[r*o.columns+c] = a[r] * b[c];
}

void apply(Vector&o,Vector const&v,Fce const&f){
  for(size_t i=0;i<v.size;++i)
    o[i] = f(v[i]);
}

class LayerBase{
  public:
    virtual Vector const&getOutput()const = 0;
};

class Input: public LayerBase{
  public:
    Vector &output;
    Input(Vector &i):output(i){}
    void setInput(Vector &i){output = i;}
    Vector const&getOutput()const override{
      return output;
    }
};

class Layer: public LayerBase{
  public:
    Layer(
        size_t/**/      /**/input /**/ /**/        ,
        size_t/**/      /**/output/**/ /**/        ,
        Fce   /**/const&/**/fce   /**/=/**/relu    ,
        Fce   /**/const&/**/gce   /**/=/**/diffRelu):
      weight      (output,input),
      bias        (output      ),
      f           (fce         ),
      g           (gce         ), 
      weightX     (output      ),
      z           (output      ),
      o           (output      ),
      weightUpdate(output,input),
      biasUpdate  (output      ),
      gz          (output      ),
      twbu        (input       )
    {

    }
    Vector const& getOutput()const override{
      return o;
    }
    Vector const& compute(Vector const&x){
      vmvmul(weightX,weight,x);
      add(z.data,weightX.data,bias.data,bias.size);
      apply(o,z,f);
      return o;
    }
    Vector const& forward(Vector const&x){
      compute(x);
      apply(gz,z,g);
      return o;
    }
    void randomize(float mmin,float mmax){
      bias.randomize(mmin,mmax);
      weight.randomize(mmin,mmax);
    }
    size_t output()const{return weight.rows;}
    size_t input()const{return weight.columns;}
    Matrix weight      ;
    Vector bias        ;
    Fce    f           ;
    Fce    g           ;
    Vector weightX     ;
    Vector z           ;
    Vector o           ;
    Matrix weightUpdate;
    Vector biasUpdate  ;
    Vector gz          ;
    Vector twbu        ;
};

class Sample{
  public:
    Sample(size_t i,size_t o):x(i),y(o){}
    Vector x;
    Vector y;
};

class Network{
  public:
    size_t inputSize = 0;
    Network(size_t i):inputSize(i),C(i){}
    std::vector<Layer>layers;
    Vector const& forward(Vector const&x){
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
    void compute(Vector&o,Vector const&i){
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
      copy(o,prev->o);
    }
    Vector C;
    void backward(Vector const&x,Vector const&y,float s){
      sub(C.data,layers.back().o.data,y.data,y.size);
      vvcmul(C.data,C.data,-2*s,C.size);

      auto const computeUpdate = [&](size_t l,Vector const&C){
        auto&ll           = layers[l];
        auto&biasUpdate   = ll.biasUpdate  ;
        auto&gz           = ll.gz          ;
        auto&weightUpdate = ll.weightUpdate;
        vvvmul(biasUpdate.data,C.data,gz.data,gz.size);
        if(l == 0)
          mvvmul(weightUpdate,biasUpdate,x);
        else
          mvvmul(weightUpdate,biasUpdate,layers[l-1].o);
      };

      computeUpdate(layers.size()-1,C);

      for(size_t i=layers.size()-1;i>0;--i){
        auto&pl = layers.at(i);
        mulTransposed(pl.twbu,pl.weight,pl.biasUpdate);
        computeUpdate(i-1,pl.twbu);
      }
    }
    void update(){
      for(auto&l:layers){
        add(l.bias.data,l.bias.data,l.biasUpdate.data,l.biasUpdate.size);
        add(l.weight,l.weight,l.weightUpdate);
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
      Vector o(output());
      Vector oy(output());
      float counter = 0;
      for(size_t i=0;i<samples.size();++i){
        //std::cerr << "TEST: " << i << "/" << samples.size() << std::endl;
        compute(o,samples[i].x);
        sub(oy.data,o.data,samples[i].y.data,o.size);
        counter += dot(oy.data,oy.data,oy.size);
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
        layers.emplace_back(layers.back().output(),n,f,g);
      C = Vector(n);
    }

    Vector operator()(Vector const&x){
      Vector y(output());
      compute(y,x);
      return y;
    }

    size_t input()const{return inputSize;}
    size_t output()const{return layers.back().output();}
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
      s.x.data[0] = randomf(mmin,mmax);
      s.x.data[1] = randomf(mmin,mmax);
      s.y.data[0] = s.x.data[0] + s.x.data[1];
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

  Vector x(2);
  x.data[0] = 1;
  x.data[1] = 2;
  std::cerr << nn(x) << std::endl;

  return 0;
}


