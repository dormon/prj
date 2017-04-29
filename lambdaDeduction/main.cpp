#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>

template<typename OUTPUT,typename...ARGS>
OUTPUT call(OUTPUT(&fce)(ARGS...),ARGS... args){
  return fce(args...);
}

template<typename F,typename...ARGS>
int32_t call2(F const&lambda,ARGS... args){
  return lambda(args...);
}

int32_t addOne(int32_t i){
  return i+1;
}

template <typename T>
struct function_traits
: public function_traits<decltype(&T::operator())>
{};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
  enum { arity = sizeof...(Args) };
  // arity is the number of arguments.

  typedef ReturnType result_type;

  template <size_t i>
    struct arg
    {
      typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
      // the i-th argument is equivalent to the i-th tuple element of a tuple
      // composed of those arguments.
    };
};

/*
// test code below:
int main()
{
  auto lambda = [](int i) { return long(i*10); };

  typedef function_traits<decltype(lambda)> traits;

  static_assert(std::is_same<long, traits::result_type>::value, "err");
  static_assert(std::is_same<int, traits::arg<0>::type>::value, "err");

  return 0;
}
*/

template<typename CLASS,typename OUTPUT,typename...ARGS>
OUTPUT callx(OUTPUT(CLASS::*fce)(ARGS...)const,ARGS...args){
  return fce(args...);
}

template<typename T>
struct ASD: public ASD<decltype(&T::operator())>{};

template<typename CLASS,typename OUTPUT,typename...ARGS>
struct ASD<OUTPUT(CLASS::*)(ARGS...)>{};

template<typename T,typename OUTPUT,typename... ARGS>
OUTPUT cally(T const&lambda,ARGS...args){
  return lambda(args...);
}


int main(){
  std::cout<<call(addOne,10)<<std::endl;
  auto lm =[](int32_t i)->int32_t{return i+1;};
  //std::cout<<callx<decltype(lm)>(lm,100)<<std::endl;
  //typedef ASD<decltype(lm)> hoho;
  cally<decltype(lm),int32_t>(lm,10);
  //std::cout<<call2([](int32_t i)->int32_t{return i+1;},100)<<std::endl;
  //std::cout<<call2(addOne,10)<<std::endl;
  return 0;
}
