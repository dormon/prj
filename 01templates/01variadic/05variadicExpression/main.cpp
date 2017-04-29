#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>



//can be used in templates
template<bool... >struct allTrue;

template<        >struct allTrue<true      >:std::true_type {};
template<        >struct allTrue<false     >:std::false_type{};

template<bool...T>struct allTrue<true ,T...>:allTrue<T...>  {};
template<bool...T>struct allTrue<false,T...>:std::false_type{};

template<typename...FLOATS,typename std::enable_if<allTrue<std::is_same<FLOATS,float>::value...>::value,int32_t>::type = 0>
float area(FLOATS const& ... floats){
  float result = 1.f;
  float a[] = { result *= floats ...};
  static_cast<void>(a);
  return result;
}

class A{
  public:
    int a;
    float s;
    int x[10];
        
};

int main(){
  std::cout << area(2.f,3.f,4.3f) << std::endl;
  return 0;
}
