#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>

int main(){
  std::vector<float>a = {0,1,2,3};
  std::vector<float*>pa;
  std::vector<float*>ppa;
  for(auto&x:a){
    pa.push_back(&x);
  }
  for(size_t i=0;i<a.size();++i)
    ppa.push_back(a.data()+i);
  if(pa != ppa)
    std::cerr << "pointer from reference is not equal to pointer to element" << std::endl;

  return 0;
}


