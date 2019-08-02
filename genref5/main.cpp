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
    ~Vector(){
      free();
    }
    float*getData(){
      return data;
    }
    float const*getData()const{
      return data;
    }
    size_t getSize()const{
      return size;
    }
    bool isAllocated()const{
      return allocated;
    }
    //float&operator[](size_t i){
    //  return data[i];
    //}
    //float const&operator[](size_t i)const{
    //  return data[i];
    //}
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
      rows = o.getRows();
      columns = o.getColumns();
      return*this;
    }
    size_t getRows()const{return rows;}
    size_t getColumns()const{return columns;}
    float&element(size_t r,size_t c){
      return data[r*columns+c];
    }
    Vector operator[](size_t r){
      return Vector(getData() + getColumns()*r,getColumns());
    }
    Vector const operator[](size_t r)const{
      return Vector(getData() + getColumns()*r,getColumns());
    }
  protected:
    size_t rows    = 0;
    size_t columns = 0;
};

std::ostream& operator<< (std::ostream& stream, const Vector& v) {
  stream << "[";
  for(size_t i=0;i<v.getSize();++i){
    if(i>0)stream << ",";
    stream << v.data[i];
  }
  stream << "]";
  return stream;
}

std::ostream& operator<< (std::ostream& stream, const Matrix& v) {
  for(size_t r=0;r<v.getRows();++r)
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

void add(Vector&v,Vector const&a,Vector const&b){
  assert(v.getSize() == a.getSize());
  assert(a.getSize() == b.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    v.data[i] = a.data[i] + b.data[i];
}

void sub(Vector&v,Vector const&a,Vector const&b){
  assert(v.getSize() == a.getSize());
  assert(a.getSize() == b.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    v.data[i] = a.data[i] - b.data[i];
}

void mul(Vector&v,Vector const&a,Vector const&b){
  assert(v.getSize() == a.getSize());
  assert(a.getSize() == b.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    v.data[i] = a.data[i] * b.data[i];
}

void mul(Vector&v,Vector const&a,float b){
  assert(v.getSize() == a.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    v.data[i] = a.data[i] * b;
}

float dot(Vector const&a,Vector const&b){
  assert(a.getSize() == b.getSize());
  float acc = 0;
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    acc += a.data[i] * b.data[i];
  return acc;
}

void copy(Vector&c,Vector const&a){
  assert(c.getSize() == a.getSize());
  auto const n = c.getSize();
  for(size_t i=0;i<n;++i)
    c.data[i] = a.data[i];
}

void add(Matrix&c,Matrix const&a,Matrix const&b){
  assert(c.getRows() == a.getRows());
  assert(c.getColumns() == b.getColumns());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    c.data[i] = a.data[i] + b.data[i];
}

void mul(Vector&c,Matrix const&a,Vector const&b){
  assert(a.getRows() == c.getSize());
  assert(a.getColumns() == b.getSize());
  auto const n = c.getSize();
  for(size_t i=0;i<n;++i)
    c.data[i] = dot(a[i],b);
}

void mulTransposed(Vector&c,Matrix const&a,Vector const&b){
  assert(a.getRows() == b.getSize());
  assert(c.getSize() == a.getColumns());
  auto const on = c.getSize();
  auto const in = b.getSize();
  for(size_t o=0;o<on;++o){
    float acc = 0;
    for(size_t i=0;i<in;++i)
      acc += a.data[i*on+o] * b.data[i];
    c.data[o] = acc;
  }
}

void componentMul(Vector&c,Vector const&a,Vector const&b){
  assert(c.getSize() == a.getSize());
  assert(c.getSize() == b.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    c.data[i] = a.data[i] * b.data[i];
}

void constMul(Vector&c,Vector const&a,float b){
  assert(c.getSize() == a.getSize());
  auto const n = a.getSize();
  for(size_t i=0;i<n;++i)
    c.data[i] = a.data[i] * b;
}

void matrixMul(Matrix&o,Vector const&a,Vector const&b){
  assert(o.getRows() == a.getSize());
  assert(o.getColumns() == b.getSize());
  auto const rn = o.getRows();
  auto const cn = o.getColumns();
  for(size_t r=0;r<rn;++r)
    for(size_t c=0;c<cn;++c)
      o.element(r,c) = a.data[r] * b.data[c];
}

void apply(Vector&o,Vector const&v,Fce const&f){
  assert(o.getSize() == v.getSize());
  auto const n = v.getSize();
  for(size_t i=0;i<n;++i)
    o.data[i] = f(v.data[i]);
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
    size_t output()const{return weight.getRows();}
    size_t input()const{return weight.getColumns();}
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


