#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>

template<typename VARIABLES,typename RETURN,typename...ARGS>
class Wrapper{
  public:
    using FCE_TYPE = RETURN(*)(VARIABLES&,ARGS...);
    Wrapper(FCE_TYPE const&f):impl(f){}
    RETURN operator()(ARGS...args){
      return impl(ths,args...);
    }
  private:
    FCE_TYPE impl;
    VARIABLES ths;
};

template<typename VARIABLES,typename RETURN,typename...ARGS>
Wrapper<VARIABLES,RETURN,ARGS...>makeWrapper(RETURN(*f)(VARIABLES&,ARGS...)){
  return Wrapper<VARIABLES,RETURN,ARGS...>(f);
}




struct FactorialVariables{
  size_t output    = 0;
  size_t lastInput    ;
};

size_t factorialCore(FactorialVariables&ths,size_t n){
  if(ths.output == 0)
    ths.lastInput = n+1;
  if(ths.lastInput == n)return ths.output;
  std::cout << "input changed from: " << ths.lastInput <<" to: " << n  << std::endl;
  ths.lastInput = n;
  ths.output = 1;
  for(size_t i=2;i<n;++i)
    ths.output *= i;
  return ths.output;
}

size_t factorial(size_t n){
  static FactorialVariables ths;

  return factorialCore(ths,n);
}

class Factorial{
  public:
    size_t operator()(size_t n){
      return factorialCore(ths,n);
    }
  private:
    FactorialVariables ths;
};

int main() {
  factorial(10);
  factorial(10);
  factorial(11);
  factorial(10);
  factorial(11);
  factorial(11);

  Factorial f;
  f(10);
  f(10);
  f(11);
  f(10);
  f(11);
  f(11);

  auto ff = makeWrapper<FactorialVariables>(factorialCore);
  ff(10);
  ff(10);
  ff(11);
  ff(10);
  ff(11);
  ff(11);
  return 0;
}
