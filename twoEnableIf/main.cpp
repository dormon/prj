#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>
template<typename A>
void print1(typename std::enable_if<std::is_integral<A>::value,A>::type a){
  std::cout<<"a: "<<a<<std::endl;
}

template<typename A>
class Print1{
  public:
    Print1(typename std::enable_if<std::is_integral<A>::value,A>::type const&a){
      std::cout<<"a: "<<a<<std::endl;
    }
};


template<typename A,typename B>
void print2(
    typename std::enable_if<std::is_integral<A>::value,A>::type a,
    typename std::enable_if<std::is_integral<B>::value,B>::type b){
  std::cout<<"a: "<<a<<std::endl;
  std::cout<<"b: "<<b<<std::endl;
}

template<typename A,typename = std::enable_if<std::is_integral<A>::value,A>>
void print2_5(A a){
  std::cout<<"a: "<<a<<std::endl;
}

template<typename A,typename std::enable_if<std::is_integral<A>::value,A>::type* = nullptr>
void print3(A a){
  std::cout<<"a: "<<a<<std::endl;
}

template<typename A,
  typename std::enable_if<std::is_integral<A>::value,A>::type* = nullptr,
  typename std::enable_if<std::is_same<A,int64_t>::value,A>::type* = nullptr>
void print4(A a){
  std::cout<<"a: "<<a<<std::endl;
}


int main(){
  Print1<int32_t>(1);
  print1<int32_t>(2);
  print2<int32_t,int32_t>(3,4u);
  print2_5(5);
  print3(6);
  print4(int64_t(7));
  return 0;
}
