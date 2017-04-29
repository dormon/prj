#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

template<typename...ARGUMENT_TYPES>
size_t foo(ARGUMENT_TYPES ... args){
  return sizeof...(args);
}

int main(){
  std::cout<<foo("z","f32","x","f32","y")<<std::endl;
  return 0;
}





