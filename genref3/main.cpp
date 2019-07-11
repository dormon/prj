#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>

using Vector = std::vector<float>;

using Matrix = std::vector<Vector>;

float dot(Vector const&a,Vector const&b){
  float res = 0.f;
  for(size_t i=0;i<a.size();++i)
    res += a[i] * b[i];
  return res;
}

Vector operator+(Vector const&a,Vector const&b){
  Vector res(a.size());
  for(size_t i=0;i<a.size();++i)
    res[i] = a[i]+b[i];
  return res;
}

Vector operator-(Vector const&a,Vector const&b){
  Vector res(a.size());
  for(size_t i=0;i<a.size();++i)
    res[i] = a[i]-b[i];
  return res;
}

Vector operator*(Matrix const&a,Vector const&b){
  Vector res;
  for(auto const&r:a)
    res.push_back(dot(r,b));
  return res;
}

Vector componentMul(Vector const&a,Vector const&b){
  Vector res(a.size());
  for(size_t i=0;i<a.size();++i)
    res[i] = a[i] * b[i];
  return res;
}

class Operator{
  public:
    virtual Vector operator()(std::vector<Vector> const&) = 0;
};

class Unary: public Operator{
  public:
    Vector operator()(std::vector<Vector>const&a)override final{
      return operator()(a[0]);
    }
    virtual Vector operator()(Vector const&) = 0;
};

class Binary: public Operator{
  public:
    Vector operator()(std::vector<Vector>const&a)override final{
      return operator()(a[0],a[1]);
    }
    virtual Vector operator()(Vector const&,Vector const&) = 0;
};

class Dot: public Binary{
  public:
    Vector operator()(Vector const&a,Vector const&b)override{
      Vector res = {0};
      for(size_t i=0;i<a.size();++i)
        res[0] += a[i] * b[i];
      return res;
    }
};

class Weights: public Unary{
  public:
    Weights(size_t input,size_t output){
      w.resize(output);
      for(auto&x:w)
        x.resize(input);
    }
    Vector operator()(Vector const&a)override{
      return w*a;
    }
    Matrix w;
};

class Bias: public Unary{
  public:
    Bias(size_t output):b(output){}
    Vector operator()(Vector const&a)override{
      return a+b;
    }
    Vector b;
};

using Function = std::function<float(float)>; 

constexpr const auto relu = [](float x){if(x>0.f)return x;return 0.f;};
constexpr const auto diffRelu = [](float x){if(x>0.f)return 1.f;return 0.f;};

class Activation: public Unary{
  public:
    Activation(Function const&f = relu,Function const g = diffRelu):fce(f),gce(g){}
    Vector operator()(Vector const&a)override{
      Vector res;
      for(auto const&x:a)
        res.push_back(fce(x));
      return res;
    }
    Function fce = relu;
    Function gce = diffRelu;
};

class Layer: public Unary{
  public:
    Layer(size_t input,size_t output,Function const&fce=relu,Function const&gce = diffRelu):w(input,output),b(output),f(fce,gce){
    }
    Vector operator()(Vector const&a)override{
      z = b(w(a));
      o = f(z);
      return o;
    }
    Weights    w;
    Bias       b;
    Activation f;
    Vector     z;
    Vector     o;
};

class Network: public Unary{
  public:
    std::vector<Layer>layers;
    Vector operator()(Vector const&a)override{
      bool first = true;
      Layer*prev;
      for(auto&l:layers){
        if(first){
          first = false;
          l(a);
        }else
          l(prev->o);
        prev = &l;
      }
      return prev->o;
    }
};

// (x+a-y)^2
// a[i+1] = a[i] - s*2(x+a[i]-y)

int main(){
  return 0;
}


