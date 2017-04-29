#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>



template<char F>
void print(){
  std::cout<<F<<std::endl;
}

template<unsigned TID>
struct wtf{};

template<>struct wtf<0>{typedef int   type;};
template<>struct wtf<1>{typedef float type;};
template<>struct wtf<2>{typedef char  type;};

int main(){
  print<'a'>();
  wtf<0>::type A=323.32f;
  std::cout<<A<<std::endl;
  return 0;
}
