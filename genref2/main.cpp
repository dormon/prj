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
    virtual ~Op(){}
    virtual T operator()(std::vector<T>const&d)const = 0;
    virtual std::string toStr(Type const&p)const = 0;
};

template<typename T = float>
class Const: public Op<T>{
  public:
    T data;
    Const(T const&d = 0):Op<T>(Op<T>::CONSTANT),data(d){}
    virtual T operator()(std::vector<T>const&d)const override{
      return data;
    }
    virtual std::string toStr(typename Op<T>::Type const&t)const override{
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
    virtual std::string toStr()const override{
      std::stringstream ss;
      ss << "x" << index;
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
    virtual T operator()(std::vector<T>const&d)const = 0;
    virtual std::string toStr()const = 0;
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
    virtual std::string toStr()const override{                 \
      std::stringstream ss;                                    \
      ss << Binary<T>::op[0]->toStr();                         \
      ss << #OP;                                               \
      ss << Binary<T>::op[1]->toStr();                         \
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

BINARY_OPERATOR(Addition      ,+)
BINARY_OPERATOR(Multiplication,*)
BINARY_OPERATOR(Subtraction   ,-)
BINARY_OPERATOR(Division      ,/)

int main(){
  //std::make_shared<Addition<float>>(nullptr,nullptr);
  auto f = (con(1.f)+var(0))*var(1);
  std::cerr << f(2,3) << std::endl;
  std::cerr << f.toStr() << std::endl;

  
  //auto o = std::make_shared<Addition<float>>();
  //std::make_shared<Op>(Var(0));
  //auto f = Var(0)+Var(1);
  return 0;
}
