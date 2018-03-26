#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

// a,b,c,...,a1,x0,... - scalars
// [a]                 - vector
// [[a]]               - vector of vectors
//
// [x[0]]   = { x0 , x1 ,...}
// [[x]]  = {[x0],[x1],...}
//
// [y0]   = { y0, y1  ,...}
// [[y]]  = {[y0],[y1],...}
//
//
// y = f<p>(x)
// g(p) = len(f<p>(x)-y)
//
// g(p) = len(f<p>(x) - y)
//
// N - number of training dataset examples
// M - size of one training dataset examples
//
// g(p) = sqrt(sum(len(f<p>(x[i])-y[i])^2,i,0,N))
// g(p) = sqrt(sum(sqrt(sum((f<p>(x[i])[j]-y[i][j])^2 ,j,0,M)^2,i,0,N))
// g(p) = sqrt(sum(sum((f<p>(x[i])[j]-y[i][j])^2 ,j,0,M),i,0,N))
// g(p) = sum((sum(f<p>(x[i])[j]-y[i][j])^2,j,0,M),i,0,N)
//
// h(p) = diff(g,p)
//
// h(p) = sum(sum(2*(f<p>(x[i])[j]-y[i][j])*diff(f,p)(x[i]),0,M),i,0,N)
//
// MIN[g(p),p] = p[n]
//
// p[0] = random
// p[i+1] = p[i] - s * diff(g,p)(p[i])
//
//
//
//
//
//
//
//


class Type{};
class EmptyType             : public Type{};
class CommonType            : public Type{};
class ParameterType         : public Type{};
class AtomicParameterType   : public ParameterType{public: std::shared_ptr<CommonType>inner;};
class CompositeParameterType: public ParameterType{};
class StructParameterType   : public CompositeParameterType{public: std::vector<std::shared_ptr<ParameterType>>elements;};
class LimitlessParameterType: public CompositeParameterType{public: std::shared_ptr<ParameterType>inner;};
class LimitedParameterType  : public LimitlessParameterType{};
class AtomicType            : public CommonType{};
class BitType               : public AtomicType{};
class AnyType               : public AtomicType{};
class CompositeType         : public CommonType{};
class StructType            : public CompositeType{public: std::vector<std::shared_ptr<CommonType>>elements;};
class LimitlessType         : public CompositeType{public: std::shared_ptr<CommonType>inner;};
class FunctionType          : public CompositeType{public: std::shared_ptr<ParameterType>parameters;};
class LimitedType           : public LimitlessType{};


class Resource{
  public:
    virtual ~Resource(){}
};

template<typename T>
class BaseResource: public Resource{
  public:
    T data;
};

class Function{
  public:
    virtual void operator()() = 0;
    std::vector<std::shared_ptr<Resource>>params;
};

class NodeFunction{
  public:
    void operator()(){
      for(auto const&x:inputs)
        (*x)();
      (*fce)();
    }
    std::set<std::shared_ptr<NodeFunction>>inputs;
    std::shared_ptr<Function>fce;
};

class GraphFunction: public Function     {
  public:
    virtual void operator()()override{
      (*root)();
    }
    std::shared_ptr<NodeFunction>root;
};

class ListFunction : public Function     {
  public:
    virtual void operator()()override{
      for(auto const&x:fces)
        (*x)();
    }
    std::vector<std::shared_ptr<Function>>fces;
};

class WhileFunction: public Function     {
  public:
    virtual void operator()()override{
      while(cond->data){
        (*body)();
      }
    }
    std::shared_ptr<Function>body;
    std::shared_ptr<BaseResource<bool>>cond;
};

class IfFunction   : public WhileFunction{
  public:
    virtual void operator()()override{
      if(cond->data){
        (*body)();
      }
    }
};

//class Function;
//
//
//using Fce = std::shared_ptr<Function>;
//
//
//class Function{
//  public:
//    virtual ~Function(){}
//};
//
//class CompositeFunction: public Function{
//  public:
//    std::vector<Fce>fces;
//};
//
//class AddExpression: public Expression{
//  public:
//    AddExpression(std::shared_ptr<Expression>const&a,std::shared_ptr<Expression>const&b):a(a),b(b){}
//    std::shared_ptr<Expression>a;
//    std::shared_ptr<Expression>b;
//};
//
//using Add = std::shared_ptr<AddExpression>;
//
//class ConstExpression: public Expression{
//  public:
//};
//
//class VarExpression: public Expression{
//  public:
//};
//
//template<typename T,typename FROM>
//std::shared_ptr<T>cast(std::shared_ptr<FROM>const&e){
//  return std::dynamic_pointer_cast<T>(e);
//}
//
//Add addCast(Exp const&e){
//  return cast<AddExpression>(e);
//}
//
//Exp operator+(Exp const&a,Exp const&b){
//  return std::make_shared<AddExpression>(a,b);
//}
//
//std::shared_ptr<Expression> diff(std::shared_ptr<Expression>const&a);
//
//std::shared_ptr<Expression> diff(std::shared_ptr<AddExpression>const&a){
//  return diff(a->a) + diff(a->b);
//}
//
//std::shared_ptr<Expression> diff(std::shared_ptr<Expression>const&a){
//  if(addCast(a))
//    return diff(addCast(a));
//  return nullptr;
//}
//
//template<typename T>
//T sum(T*p,size_t N){
//  T result = 0;
//  for(size_t i=0;i<N;++i)
//    result = result + p[i];
//  return result;
//}
//

int main(){
  return 0;
}





