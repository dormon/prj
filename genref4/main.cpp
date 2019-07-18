#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>
#include <time.h>
#include <chrono>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

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


class Vector: public std::vector<float>{
  public:
    Vector(){}
    Vector(size_t l){
      resize(l,0);
    }
    void randomize(float mmin,float mmax){
      for(auto&x:*this)
        x = randomf(mmin,mmax);
    }
};


std::ostream& operator<< (std::ostream& stream, const Vector& v) {
  stream << "[";
  bool first = true;
  for(auto const&e:v){
    if(first)first = false;
    else stream << ",";
    stream << e;
  }

  stream << "]";
  return stream;
}


class Matrix: public std::vector<Vector>{
  public:
    Matrix(size_t rows,size_t columns){
      assert(rows > 0);
      assert(columns > 0);
      resize(rows);
      for(auto&x:*this)
        x.resize(columns,0);
    }
    size_t rows()const{
      return size();
    }
    size_t columns()const{
      return this->at(0).size();
    }
    void randomize(float mmin,float mmax){
      for(auto&r:*this)
        r.randomize(mmin,mmax);
    }
};


std::ostream& operator<< (std::ostream& stream, const Matrix& v) {
  for(size_t r=0;r<v.rows();++r)
    stream << v[r] << std::endl;
  return stream;
}

void transpose(Matrix&o,Matrix const&i){
  assert(o.rows() == i.columns());
  assert(o.columns() == i.rows());
  for(size_t r=0;r<i.rows();++r)
    for(size_t c=0;c<i.columns();++c)
      o[c][r] = i[r][c];
}

float dot(Vector const&a,Vector const&b){
  assert(a.size() == b.size());
  float res = 0.f;
  for(size_t i=0;i<a.size();++i)
    res += a[i] * b[i];
  return res;
}

void add(Vector&c,Vector const&a,Vector const&b){
  assert(c.size() == a.size());
  assert(c.size() == b.size());
  for(size_t i=0;i<a.size();++i)
    c[i] = a[i]+b[i];
}

void copy(Vector&c,Vector const&a){
  assert(c.size() == a.size());
  for(size_t i=0;i<c.size();++i)
    c[i] = a[i];
}

void add(Matrix&c,Matrix const&a,Matrix const&b){
  assert(c.rows() == a.rows());
  assert(c.rows() == b.rows());
  for(size_t r=0;r<c.rows();++r){
    add(c[r],a[r],b[r]);
  }
}

void sub(Vector&c,Vector const&a,Vector const&b){
  assert(c.size() == a.size());
  assert(c.size() == b.size());
  for(size_t i=0;i<a.size();++i)
    c[i] = a[i]-b[i];
}

void mul(Vector&c,Matrix const&a,Vector const&b){
  assert(a.size() == c.size());
  //std::cerr << "mul: [" << a.columns() << "," << a.rows() << "] x " << b.size() << std::endl;
  for(size_t i=0;i<c.size();++i)
    c[i] = dot(a[i],b);
}

void mulTransposed(Vector&c,Matrix const&a,Vector const&b){
  assert(a.rows() == b.size());
  assert(c.size() == a.columns());
  for(size_t o=0;o<c.size();++o){
    float acc = 0;
    for(size_t i=0;i<b.size();++i)
      acc += a[i][o] * b[i];
    c[o] = acc;
  }
}

void componentMul(Vector&c,Vector const&a,Vector const&b){
  assert(c.size() == a.size());
  assert(c.size() == b.size());
  //std::cerr << "componentMul: " << a.size() << " " << b.size() << std::endl;
  for(size_t i=0;i<a.size();++i)
    c[i] = a[i] * b[i];
}

void constMul(Vector&c,Vector const&a,float b){
  assert(c.size() == a.size());
  for(size_t i=0;i<a.size();++i)
    c[i] = a[i] * b;
}

void matrixMul(Matrix&c,Vector const&a,Vector const&b){
  assert(c.size() == a.size());
  //std::cerr << "matrixMul: " << a.size() << " x " << b.size() << std::endl;
  for(size_t r=0;r<c.size();++r)
    constMul(c[r],b,a[r]);
}

using Fce = std::function<float(float)>; 

constexpr const auto relu = [](float x){if(x>=0.f)return x;return x*0.1f;};
constexpr const auto diffRelu = [](float x){if(x>=0.f)return 1.f;return 0.1f;};

void apply(Vector&o,Vector&v,Fce const&f){
  assert(o.size() == v.size());
  for(size_t i=0;i<v.size();++i)
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
      tw          (input,output),
      twbu        (input       )
    {

    }
    Vector const& getOutput()const override{
      return o;
    }
    Vector const& compute(Vector const&x){
      mul(weightX,weight,x);
      add(z,weightX,bias);
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
    size_t output()const{return weight.rows();}
    size_t input()const{return weight.columns();}
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
    Matrix tw          ;
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
    Network(size_t i):inputSize(i){}
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
      sub(C,layers.back().o,y);
      constMul(C,C,-2*s);

      auto const computeUpdate = [&](size_t l,Vector const&C){
        componentMul(layers[l].biasUpdate,C,layers[l].gz);
        if(l == 0)
          matrixMul(layers[l].weightUpdate,layers[l].biasUpdate,x);
        else
          matrixMul(layers[l].weightUpdate,layers[l].biasUpdate,layers[l-1].o);
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
        add(l.bias,l.bias,l.biasUpdate);
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
        sub(oy,o,samples[i].y);
        counter += dot(oy,oy);
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
      C.resize(n);
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
  x[0] = 1;
  x[1] = 2;
  std::cerr << nn(x) << std::endl;

  return 0;
}


