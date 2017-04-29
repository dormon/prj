#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

template<typename...ARGS>
void toVector(std::vector<unsigned>&a,ARGS...args){
  a={args...};
}

int main(){
  std::vector<unsigned>asd;
  toVector(asd,1u,2u,3u,4u,5u);
  std::cout<<asd[0]<<std::endl;
  std::cout<<asd[1]<<std::endl;
  std::cout<<asd[2]<<std::endl;
  return 0;
}
