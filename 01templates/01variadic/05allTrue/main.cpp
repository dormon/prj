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

//cannot be used in templates
inline bool allTrue2(){return true;}

template<typename... TAIL>
inline bool allTrue2(bool const& first,TAIL const& ... tail){
  return first && allTrue2(tail...);
}



//can be used in templates
constexpr inline bool allTrue3(){return true;}

template<typename... TAIL>
constexpr inline bool allTrue3(bool const& first,TAIL const& ... tail){
  return first && allTrue2(tail...);
}


template<typename...FLOATS,typename std::enable_if<allTrue<std::is_same<FLOATS,float>::value...>::value,int32_t>::type = 0>
float area(FLOATS const& ... ){
  return 42.f;
}


int main(){
  if(allTrue<true,true,true>::value){
    std::cout<<"all true"<<std::endl;
  }else{
    std::cout<<"any false"<<std::endl;
  }

  if(allTrue2(true,true,true)){
    std::cout<<"all true"<<std::endl;
  }else{
    std::cout<<"any false"<<std::endl;
  }

  std::cout << area(2.f,3.f,4.3f) << std::endl;
  return 0;
}
