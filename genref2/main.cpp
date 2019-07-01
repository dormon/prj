#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>
#include<limits>
#include <glm/glm.hpp>

template<typename T = float>
class Op{
  public:
    enum Type{
      CONSTANT      ,
      VARIABLE      ,
      ADDITION      ,
      MULTIPLICATION,
      SUBTRACTION   ,
      DIVISION      ,
    }type;
    Op(Type const&t):type(t){}
    bool brackets(Type const&o,bool left)const{
      auto const right = !left;
      if((type == ADDITION || type == SUBTRACTION) && (o == MULTIPLICATION || o == DIVISION) )return true;
      if(left == false && o == SUBTRACTION && (type == ADDITION || type == SUBTRACTION))return true;
      if(right && o == DIVISION && type == MULTIPLICATION)return true;
      return false;
    }
    virtual ~Op(){}
    virtual T operator()(std::vector<T>const&d)const = 0;
    virtual std::string toStr(Type p = CONSTANT,bool left = true)const = 0;
};

template<typename T = float>
class Const: public Op<T>{
  public:
    T data;
    Const(T const&d = 0):Op<T>(Op<T>::CONSTANT),data(d){}
    virtual T operator()(std::vector<T>const&d)const override{
      return data;
    }
    virtual std::string toStr(typename Op<T>::Type = Op<T>::CONSTANT,bool = true)const override{
      std::stringstream ss;
      ss << data;
      return ss.str();
    }
};

template<typename T=float>
Const<T> con(T const&d=0){
  return Const<T>(d);
}

template<typename T = float>
class Var: public Op<T>{
  public:
    size_t index;
    Var(size_t i = 0):Op<T>(Op<T>::VARIABLE),index(i){}
    virtual ~Var(){}
    virtual T operator()(std::vector<T>const&d)const override{
      return d[index];
    }
    virtual std::string toStr(typename Op<T>::Type = Op<T>::CONSTANT,bool = true)const override{
      std::stringstream ss;
      ss << "x[" << index << "]";
      return ss.str();
    }
};

template<typename T=float>
Var<T> var(size_t i=0){
  return Var<T>(i);
}


template<typename T = float>
class Binary: public Op<T>{
  public:
    std::shared_ptr<Op<T>>op[2];
    Binary(typename Op<T>::Type const&t):Op<T>(t){}
    virtual ~Binary(){}
};


#define BINARY(NAME,TYPE,OP)                                   \
template<typename T = float>                                   \
class NAME: public Binary<T>{                                  \
  public:                                                      \
    NAME():Binary<T>(Op<T>::TYPE){}                            \
    NAME(std::shared_ptr<Op<T>>const&a,                        \
         std::shared_ptr<Op<T>>const&b):NAME(){                \
      Binary<T>::op[0] = a;                                    \
      Binary<T>::op[1] = b;                                    \
    }                                                          \
    virtual T operator()(std::vector<T>const&d)const override{ \
      return (*Binary<T>::op[0])(d) OP (*Binary<T>::op[1])(d); \
    }                                                          \
    template<typename...ARGS>                                  \
    T operator()(ARGS const&...args)const{                     \
      std::vector<T>v = {static_cast<T>(args)...};             \
      return operator()(v);                                    \
    }                                                          \
    virtual std::string toStr(typename Op<T>::Type t = Op<T>::CONSTANT,bool left = true)const override{\
      std::stringstream ss;                                    \
      if(Op<T>::brackets(t,left))ss << "(";\
      ss << Binary<T>::op[0]->toStr(Op<T>::type,true);                         \
      ss << #OP;                                               \
      ss << Binary<T>::op[1]->toStr(Op<T>::type,false);                         \
      if(Op<T>::brackets(t,left))ss << ")";\
      return ss.str();                                         \
    }                                                          \
}

BINARY(Addition      ,ADDITION      ,+);
BINARY(Multiplication,MULTIPLICATION,*);
BINARY(Subtraction   ,SUBTRACTION   ,-);
BINARY(Division      ,DIVISION      ,/);

template<template<typename...>class A,typename...ARGS>
std::shared_ptr<A<ARGS...>>toShared(A<ARGS...>const&a){
  return std::make_shared<A<ARGS...>>(a);
}

#define BINARY_OPERATOR(CLASS,OP)\
template<typename T = float,template<typename...>class A,template<typename...>class B>\
CLASS<T> operator OP (A<T>const&a,B<T>const&b){\
  return CLASS<T>(toShared(a),toShared(b));\
}

BINARY_OPERATOR(Addition      ,+);
BINARY_OPERATOR(Multiplication,*);
BINARY_OPERATOR(Subtraction   ,-);
BINARY_OPERATOR(Division      ,/);


template<template<typename...>class TO,typename T=float>
std::shared_ptr<TO<T>>cast(std::shared_ptr<Op<T>>const&a){
  return std::dynamic_pointer_cast<TO<T>>(a);
}

template<typename T=float>
std::shared_ptr<Op<T>>diff(std::shared_ptr<Op<T>>const&o,size_t i){
  if(o->type == Op<T>::CONSTANT)return std::make_shared<Const<T>>();
  if(o->type == Op<T>::VARIABLE){
    if(cast<Var,T>(o)->index == i)return std::make_shared<Const<T>>(1.f);
    return std::make_shared<Const<T>>();
  }
  if(o->type == Op<T>::ADDITION){
    auto ad = cast<Addition,T>(o);
    return std::make_shared<Addition<T>>(diff(ad->op[0],i),diff(ad->op[1],i));
  }
  if(o->type == Op<T>::SUBTRACTION){
    auto ad = cast<Subtraction,T>(o);
    return std::make_shared<Subtraction<T>>(diff(ad->op[0],i),diff(ad->op[1],i));
  }
  if(o->type == Op<T>::MULTIPLICATION){
    auto oo = cast<Multiplication,T>(o);
    auto a = oo->op[0];
    auto b = oo->op[1];
    auto ad = diff(a,i);
    auto bd = diff(b,i);
    auto adb = std::make_shared<Multiplication<T>>(ad,b);
    auto abd = std::make_shared<Multiplication<T>>(a,bd);
    return std::make_shared<Addition<T>>(adb,abd);
  }
  if(o->type == Op<T>::DIVISION){
    auto oo = cast<Division,T>(o);
    auto a = oo->op[0];
    auto b = oo->op[1];
    auto ad = diff(a,i);
    auto bd = diff(b,i);
    auto adb = std::make_shared<Multiplication<T>>(ad,b);
    auto abd = std::make_shared<Multiplication<T>>(a,bd);
    auto top = std::make_shared<Subtraction<T>>(adb,abd);
    auto bottom = std::make_shared<Multiplication<T>>(b,b);
    return std::make_shared<Division<T>>(top,bottom);
  }
}

template<typename T = float>
Subtraction<T> diff(Subtraction<T>const&t,size_t i);

template<typename T = float>
Addition<T> diff(Addition<T>const&t,size_t i);

template<typename T = float>
Addition<T> diff(Multiplication<T>const&t,size_t i);

template<typename T>
Addition<T> diff(Addition<T>const&t,size_t i){
  return diff(*t.op[0],i)+diff(*t.op[1],i);
}

template<typename T>
Subtraction<T> diff(Subtraction<T>const&t,size_t i){
  return diff(*t.op[0],i)-diff(*t.op[1],i);
}
  

template<typename T>
Addition<T> diff(Multiplication<T>const&t,size_t i){
  return diff(*t.op[0],i)*(*t.op[1])+(*t.op[0])*diff(*t.op[1],i);
}


int main(){
  //std::make_shared<Addition<float>>(nullptr,nullptr);
  auto f = (con(1.f)+var(0))*var(1);
  std::cerr << f(2,3) << std::endl;
  std::cerr << f.toStr() << std::endl;
  std::cerr << (var(0)+var(1)+var(2)).toStr() << std::endl;
  std::cerr << ((var(0)-var(1))-var(2)).toStr() << std::endl;
  std::cerr << (var(0)-(var(1)-var(2))).toStr() << std::endl;
  std::cerr << (var(0)-(var(1)*var(2))).toStr() << std::endl;
  std::cerr << (var(0)-(var(1)*var(2))).toStr() << std::endl;
  std::cerr << (var(0)/(var(1)*var(2))).toStr() << std::endl;
  std::cerr << ((var(0)/var(1))*var(2)).toStr() << std::endl;
  std::cerr << (var(0)/(var(1)+var(2))).toStr() << std::endl;
  std::cerr << (var(0)/(var(1)-var(2))).toStr() << std::endl;
  std::cerr << ((var(1)+var(2))/var(0)).toStr() << std::endl;
  std::cerr << ((var(1)-var(2))/var(0)).toStr() << std::endl;

  //std::cerr << diff(var(0)+var(1),0).toStr() << std::endl;

  std::shared_ptr<Op<float>>a;
  a = std::make_shared<Const<float>>();
  cast<Const>(a);
  //auto o = std::make_shared<Addition<float>>();
  //std::make_shared<Op>(Var(0));
  //auto f = Var(0)+Var(1);
  return 0;
}
