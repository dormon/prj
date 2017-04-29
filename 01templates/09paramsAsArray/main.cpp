#include<iostream>
#include<vector>

template<size_t... Is>
struct seq { };

template<size_t N, size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

float add1f(float a                        ){return a      ;}
float add2f(float a,float b                ){return a+b    ;}
float add3f(float a,float b,float c        ){return a+b+c  ;}
float add4f(float a,float b,float c,float d){return a+b+c+d;}

template<typename...ARGUMENTS,size_t...I>
float call_helper(
    float(*fce)(ARGUMENTS...),
    std::vector<float> const& params,
    seq<I...>){
  return fce(params.at(I)...);
}

template<typename...ARGUMENTS>
float call(
    float(*fce)(ARGUMENTS...),
    std::vector<float> const& params){
  return call_helper(fce,params,gen_seq<sizeof...(ARGUMENTS)>{});
}

int main(){

  std::vector<float> params = {1.f,2.f,3.f,4.f};
  std::cout << call(add1f,params) << std::endl;
  std::cout << call(add2f,params) << std::endl;
  std::cout << call(add3f,params) << std::endl;
  std::cout << call(add4f,params) << std::endl;

  return 0;
}

